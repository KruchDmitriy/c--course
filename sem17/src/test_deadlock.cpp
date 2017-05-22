#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t shared_mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t shared_mutex2 = PTHREAD_MUTEX_INITIALIZER;

void* worker_func1(void *ignored) {
    pthread_mutex_lock(&shared_mutex1);
    sleep(1);
    pthread_mutex_lock(&shared_mutex2);
    pthread_mutex_unlock(&shared_mutex2);
    pthread_mutex_unlock(&shared_mutex1);
    return NULL;
}

void* worker_func2(void *ignored) {
    pthread_mutex_lock(&shared_mutex2);
    sleep(1);
    pthread_mutex_lock(&shared_mutex1);
    pthread_mutex_unlock(&shared_mutex1);
    pthread_mutex_unlock(&shared_mutex2);
    return NULL;
}

int main() {
    pthread_attr_t thread_attrs;
    pthread_attr_init(&thread_attrs);
    pthread_t thread1, thread2;

    pthread_create(&thread1, &thread_attrs, &worker_func1, NULL);
    pthread_create(&thread2, &thread_attrs, &worker_func2, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return EXIT_SUCCESS;
}
