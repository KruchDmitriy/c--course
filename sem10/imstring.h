#ifndef IMSTRING_H
#define IMSTRING_H

#include <string>
#include <string.h>
#include <memory>
#include <ostream>
#include <list>
#include <iostream>

class imstring {
private:
    struct node {
        node() :is_leaf(true) {}

        node(std::shared_ptr<node> left, std::shared_ptr<node> right)
            : left(left)
            , right(right)
            , is_leaf(false) {}

        node(const char* str)
            : is_leaf(true)
        {
            char* buf_ = new char[strlen(str) + 1];
            std::copy(str, str + strlen(str) + 1, buf_);
            buffer = std::shared_ptr<char>(buf_, [](char * s){ delete []s; });
        }

        node(const node&) = default;
        node(node&&) = default;

        node& operator =(const node &) = default;
        node& operator =(node &&) = default;

        std::shared_ptr<node> left;
        std::shared_ptr<node> right;
        std::shared_ptr<char> buffer;
        const bool is_leaf;
    };

    bool is_cached() {
        return (bool) node_->buffer;
    }

    char* get_buffer() {
        return node_->buffer.get();
    }

    std::shared_ptr<node> node_;
    const size_t size_;

public:
    imstring() : size_(0) {}

    imstring(const char* str)
        : node_(std::make_shared<node>(str))
        , size_(strlen(str)) {}

    imstring(const imstring& left, const imstring& right)
        : node_(std::make_shared<node>(left.node_, right.node_))
        , size_(left.size() + right.size()) {}

    imstring(const imstring& istr)
        : node_(istr.node_)
        , size_(istr.size_)
    {}

    imstring(imstring&& istr)
        : node_(std::move(istr.node_))
        , size_(istr.size_)
    {}

    imstring& operator=(const imstring&) = delete;
    imstring& operator=(imstring&&) = delete;

    class imstring_iterator {
    private:
        using base = std::iterator<std::forward_iterator_tag, char>;
        using list_type = std::list<node*>;

        list_type list;
        list_type::iterator cur_it;
        char* cur_str;

        void made_list(node* node_) {
            list.push_back(node_);

            for (auto it = list.begin(); it != list.end(); ++it) {
                node* cur_node = *it;
                if (!cur_node->is_leaf) {
                    list.insert(it, cur_node->left.get());
                    list.insert(it, cur_node->right.get());
                    auto prev_it = it;
                    --prev_it;
                    --prev_it;
                    --prev_it;
                    list.erase(it);
                    it = prev_it;
                }
            }
        }

        node* get_rightmost(node* node_) {
            while (!node_->is_leaf) {
                node_ = node_->right.get();
            }

            return node_;
        }

    public:
        using value_type = char;
        using pointer    = base::pointer;
        using reference  = base::reference;
        using difference_type   = base::difference_type;
        using iterator_category = base::iterator_category;

        imstring_iterator() {}

        imstring_iterator(node* node_, bool is_end = false) {
            cur_it = list.begin();

            if (is_end) {
                node* right = get_rightmost(node_);
                cur_str = right->buffer.get();
                cur_str += strlen(cur_str);
            } else {
                made_list(node_);
                cur_it = list.begin();
                cur_str = list.front()->buffer.get();
            }
        }

        imstring_iterator(const imstring_iterator& it)
            : list(it.list)
            , cur_str(it.cur_str)
        {
            list_type::const_iterator cur = it.cur_it;
            size_t shift = std::distance(it.list.cbegin(), cur);
            cur_it = list.begin();
            std::advance(cur_it, shift);
        }

        imstring_iterator(imstring_iterator&& it)
            : cur_str(it.cur_str)
        {
            list_type::const_iterator cur = it.cur_it;
            size_t shift = std::distance(it.list.cbegin(), cur);
            list = std::move(it.list);
            cur_it = list.begin();
            std::advance(cur_it, shift);

            it.cur_str = nullptr;
        }

        imstring_iterator& operator= (const imstring_iterator&) = delete;
        imstring_iterator& operator= (imstring_iterator&&)      = delete;

        imstring_iterator& operator++() {
            cur_str++;
            if (*cur_str == '\0' && cur_it != --list.end()) {
                ++cur_it;
                cur_str = (*cur_it)->buffer.get();
            }

            return *this;
        }

        imstring_iterator operator++(int) {
            imstring_iterator tmp(*this);
            operator ++();
            return tmp;
        }

        const char& operator*() {
            return *cur_str;
        }

        bool operator ==(const imstring_iterator& other) {
            return cur_str == other.cur_str;
        }

        bool operator !=(const imstring_iterator& other) {
            return !(operator ==(other));
        }
    };

    const char* c_str() {
        if (!node_) {
            return "";
        }

        if (node_->buffer) {
            return node_->buffer.get();
        }

        char* buffer = new char[size_ + 1];

        std::copy(begin(), end(), buffer);
        buffer[size_] = '\0';

        node_->buffer = std::shared_ptr<char>(buffer, [](char* str) { delete[] str; });
        return node_->buffer.get();
    }

    imstring operator+(const imstring& other) {
        return imstring(*this, other);
    }

    friend std::ostream& operator <<(std::ostream& os, imstring& istr) {
        if (istr.is_cached()) {
            os << istr.get_buffer();
            return os;
        }

        auto end_= istr.end();
        for (auto it = istr.begin(); it != end_; ++it) {
            os << *it;
        }

        return os;
    }

    size_t size() const {
        return size_;
    }

    imstring_iterator begin() {
        return imstring_iterator(node_.get());
    }

    imstring_iterator end() {
        return imstring_iterator(node_.get(), true);
    }
};

#endif // IMSTRING_H
