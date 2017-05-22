#include <pthread.h>
#include <dlfcn.h>
#include <unordered_map>
#include <iostream>
#include <algorithm>

extern "C" {

    typedef int (*pthread_mutex_func_t)(pthread_mutex_t*);
    typedef int (*pthread_create_func_t)(pthread_t *, const pthread_attr_t *,
                          void *(*start_routine) (void *), void *);
    typedef void (*pthread_exit_func_t)(void *);

    pthread_mutex_func_t  __pthread_mutex_lock;
    pthread_mutex_func_t  __pthread_mutex_unlock;

    pthread_mutex_t shared_mutex;
    std::unordered_map<std::string, int>* locked_mutexes;
    std::unordered_map<pthread_t, pthread_mutex_t*>* thread_wait;
    std::unordered_map<pthread_mutex_t*, pthread_t>* mutex_owner;
    std::unordered_map<pthread_t, std::vector<pthread_mutex_t*>>* order;

    static void __attribute__ ((constructor)) init(void) noexcept {
        printf("dlopen\n");
        void* handle = dlopen("libpthread.so.0", RTLD_LOCAL | RTLD_LAZY);

        if (!handle) {
            printf("dlopen failed, error: %s\n", dlerror());
            return;
        }

        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&shared_mutex, &attr);

        locked_mutexes = new std::unordered_map<std::string, int>();
        thread_wait    = new std::unordered_map<pthread_t, pthread_mutex_t*>();
        mutex_owner    = new std::unordered_map<pthread_mutex_t*, pthread_t>();
        order          = new std::unordered_map<pthread_t, std::vector<pthread_mutex_t*>>();

        __pthread_mutex_lock   = (pthread_mutex_func_t)  dlsym(handle, "pthread_mutex_lock");
        __pthread_mutex_unlock = (pthread_mutex_func_t)  dlsym(handle, "pthread_mutex_unlock");
    }


    static void __attribute__ ((destructor)) tear_down(void) noexcept {
        delete locked_mutexes;
        delete thread_wait;
        delete mutex_owner;
        delete order;
    }


    void check_for_deadlock(pthread_mutex_t *mutex, pthread_t tid) noexcept {
        auto thread_it = mutex_owner->find(mutex);
        if (thread_it == mutex_owner->end()) {
            return;
        }

        printf("mutex is locked, found by whom: %ld\n", thread_it->second);
        fflush(stdout);
        auto mutex_wait = thread_wait->find(thread_it->second);

        thread_wait->insert(std::make_pair(tid, mutex));

        if (mutex_wait != thread_wait->end()) {
            printf("do you wait somebody?: %p\n", mutex_wait->second);

            auto blame_thread_id = mutex_owner->find(mutex_wait->second);

            if (blame_thread_id != mutex_owner->end() &&
                blame_thread_id->second == tid)
            {
                fprintf(stderr, "Deadlock, threads: %ld and %ld\n", tid, thread_it->second);
                fflush(stdout);
            }
        }
    }


    int pthread_mutex_lock(pthread_mutex_t *mutex) noexcept {
        const pthread_t tid = pthread_self();
        const std::string id = "mutex:" + std::to_string(reinterpret_cast<uint64_t>(mutex)) + "tid:" + std::to_string(tid);


        __pthread_mutex_lock(&shared_mutex);
        auto it = locked_mutexes->find(id);
        if (it == locked_mutexes->end()) {
            if (pthread_mutex_trylock(mutex) != 0) {
                check_for_deadlock(mutex, tid);
            } else {
                __pthread_mutex_unlock(mutex);
            }
            __pthread_mutex_unlock(&shared_mutex);


            int error = __pthread_mutex_lock(mutex);


            printf("mutex locked: %p, by thread %ld\n", mutex, tid);

            __pthread_mutex_lock(&shared_mutex);

                locked_mutexes->insert(std::make_pair(id, 1));
                mutex_owner->insert(std::make_pair(mutex, tid));
                (*order)[tid].push_back(mutex);

                auto it = thread_wait->find(tid);
                if (it != thread_wait->end()) {
                    thread_wait->erase(it);
                }

            __pthread_mutex_unlock(&shared_mutex);

            return error;
        }

        it->second = it->second + 1;
        fprintf(stderr, "lock of already locked mutex %p, thread_id = %ld\n", mutex, tid);
        __pthread_mutex_unlock(&shared_mutex);

        return 0;
    }


    int pthread_mutex_unlock(pthread_mutex_t *mutex) noexcept {
        __pthread_mutex_lock(&shared_mutex);

            const pthread_t tid = pthread_self();
            const std::string id = "mutex:"
                + std::to_string(reinterpret_cast<uint64_t>(mutex))
                + "tid:" + std::to_string(tid);

            auto it = locked_mutexes->find(id);
            if (it == locked_mutexes->end()) {
                __pthread_mutex_unlock(&shared_mutex);
                return 0;
            }

            it->second = it->second - 1;

            if (it->second > 0) {
                __pthread_mutex_unlock(&shared_mutex);
                return 0;
            }

            locked_mutexes->erase(it);
            mutex_owner->erase(mutex_owner->find(mutex));

            if ((*order)[tid].back() != mutex) {
                fprintf(stderr, "wrong order of frees\n");
            }
            (*order)[tid].pop_back();

        __pthread_mutex_unlock(&shared_mutex);

        int error = __pthread_mutex_unlock(mutex);


        __pthread_mutex_lock(&shared_mutex);

            printf("mutex unlocked: %p, by thread %ld\n", mutex, tid);

        __pthread_mutex_unlock(&shared_mutex);


        return error;
    }
}
