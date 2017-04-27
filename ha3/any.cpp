#include "any.h"

namespace utils
{
    any::any() : content(0) {}

    any::~any()
    {
        if (content) delete content;
    }

    any::any(const any& other)
    {
        content = nullptr;
        if (other.content) {
            content = other.content->copy();
        }
    }

    any& any::swap(any& other)
    {
        std::swap(content, other.content);
        return *this;
    }

    bool any::empty()
    {
        return !content;
    }

    any& any::operator = (any other)
    {
        swap(other);
        return *this;
    }

    void swap(any& left, any& right)
    {
        left.swap(right);
    }
}
