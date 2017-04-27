#pragma once
#include <memory>
#include <stdexcept>
#include <cassert>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <utility>

namespace details {

template<class KEY, class VALUE>
struct node {
private:
    KEY key_;
    VALUE value_;
    node* parent_node;
    std::unique_ptr<node> left_child;
    std::unique_ptr<node> right_child;

public:
    node(const KEY &key, const VALUE &val)
    : key_(key)
    , value_(val)
    , parent_node(nullptr)
    {}

    ~node() noexcept {}

    // only move semantics are possible
    void putLeft(std::unique_ptr<node> &&nptr) noexcept {
        left_child = std::move(nptr);
        left_child->parent_node = this;
    }

    std::unique_ptr<node> takeLeft() noexcept {
        if (left_child) {
            left_child->parent_node = nullptr;
        }
        return std::move(left_child);
    }

    void putRight(std::unique_ptr<node> &&nptr) noexcept {
        right_child = std::move(nptr);
        right_child->parent_node = this;
    }

    std::unique_ptr<node> takeRight() noexcept {
        if (right_child) {
            right_child->parent_node = nullptr;
        }
        return std::move(right_child);
    }

    node*& parent() noexcept {
        return parent_node;
    }

    node* left() noexcept {
        if (left_child) {
            return left_child.get();
        }
        return nullptr;
    }

    node* right() noexcept {
        if (right_child) {
            return right_child.get();
        }
        return nullptr;
    }

    KEY& key() noexcept {
        return key_;
    }

    VALUE &value() noexcept {
        return value_;
    }
};

} // namespace details

struct bst_key_exists_exception : public std::exception {
    bst_key_exists_exception() {}
    virtual const char* what() const noexcept {
        return "Key already exists.";
    }
};

struct named_bst_key_exists_exception : public bst_key_exists_exception {
private:
    std::string key;
public:
    named_bst_key_exists_exception(std::string&& key)
    : key(std::move(key))
    {}

    virtual const char* what() const noexcept {
       return key.c_str();
    }
};

struct bst_key_absent_exception : public std::exception {
    bst_key_absent_exception() {}
    virtual const char* what() const noexcept {
        return "Tree has no value with such key.";
    }
};

// KEY should have operator <, <<, () constructor
template<class KEY, class VALUE>
struct bin_search_tree {
private:
    using node = details::node<KEY, VALUE>;
    std::unique_ptr<node> root;

    node* find_node(const KEY &key) {
        if (!(bool) root) return nullptr;

        node* cur_node = root.get();
        node* prev_node = nullptr;
        while (cur_node != nullptr) {
            if (key == cur_node->key()) {
                return cur_node;
            }

            if (key < cur_node->key()) {
                prev_node = cur_node;
                cur_node = cur_node->left();
            } else {
                prev_node = cur_node;
                cur_node = cur_node->right();
            }
        }

        return prev_node;
    }

    void insert(std::unique_ptr<node>&& node_) {
        if (!(bool) root) {
            root = std::move(node_);
        } else {
            node* nearest_node = find_node(node_->key());

            if (node_->key() < nearest_node->key()) {
                nearest_node->putLeft(std::move(node_));
            } else {
                nearest_node->putRight(std::move(node_));
            }
        }
    }

public:
    bin_search_tree() {}

    ~bin_search_tree() {
        bool is_bottom_up = false;
        node* cur_node = root.get();

        if (cur_node == nullptr)
            return;

        while (true) {
            if (!cur_node) {
                return;
            }

            if (is_bottom_up) {
                if (cur_node->left()) {
                    cur_node->takeLeft();
                } else {
                    cur_node->takeRight();
                }
                is_bottom_up = false;
            }

            if (cur_node->left()) {
                cur_node = cur_node->left();
            } else if (cur_node->right()) {
                cur_node = cur_node->right();
            } else {
                is_bottom_up = true;
                cur_node = cur_node->parent();
            }
        }
    }

    // throws bst_key_exists_exception
    void insert(const KEY &key, const VALUE &val) {
        if (contains(key)) {
            std::stringstream ss;
            ss << key;
            throw named_bst_key_exists_exception(ss.str());
        }

        insert(std::make_unique<node>(key, val));
    }

    // throws bst_key_exists_exception
    template<class KIT, class VIT>
    void insert(KIT kbegin, KIT kend, VIT vbegin) {
        std::vector<std::unique_ptr<node>> tmp_vector;

        VIT vit = vbegin;
        for (KIT kit = kbegin; kit != kend; ++kit, ++vit) {
            tmp_vector.emplace_back(std::make_unique<node>(*kit, *vit));
            if (contains(*kit)) {
                std::stringstream ss;
                ss << *kit;
                throw named_bst_key_exists_exception(ss.str());
            }
        }

        for (auto&& item : tmp_vector) {
            insert(std::move(item));
        }
    };

    // throws bst_key_absent_exception
    VALUE& find(const KEY &key) {
        if (!contains(key)) {
            throw bst_key_absent_exception();
        }

        return find_node(key)->value();
    }

    bool contains(const KEY &key) {
        node* found = find_node(key);
        return found && found->key() == key;
    }
};
