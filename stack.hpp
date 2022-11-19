/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include <vector.hpp>

namespace ft
{
    template <typename T, typename TContainer = ft::vector<T> >
    class stack
    {
    public:
        typedef TContainer container_type;
        typedef typename TContainer::value_type value_type;
        typedef typename TContainer::size_type size_type;
        typedef typename TContainer::reference reference;
        typedef typename TContainer::const_reference const_reference;

    protected:
        container_type c;

    public:
        explicit stack(const container_type& c = container_type())
            : c(c) {}

        stack(const stack& that)
            : c(that.c) {}

        ~stack() {}

        stack& operator=(const stack& that)
        {
            this->c = that.c;
            return *this;
        }

    public:
        reference top() { return c.back(); }
        const_reference top() const { return c.back(); }

        bool empty() const { return c.empty(); }
        size_type size() const { return c.size(); }

        void push(const value_type& value) { c.push_back(value); }
        void pop() { c.pop_back(); }

    public:
        friend bool operator==(const stack& lhs, const stack& rhs)
        {
            return lhs.c == rhs.c;
        }

        friend bool operator!=(const stack& lhs, const stack& rhs)
        {
            return lhs.c != rhs.c;
        }

        friend bool operator<(const stack& lhs, const stack& rhs)
        {
            return lhs.c < rhs.c;
        }

        friend bool operator<=(const stack& lhs, const stack& rhs)
        {
            return lhs.c <= rhs.c;
        }

        friend bool operator>(const stack& lhs, const stack& rhs)
        {
            return lhs.c > rhs.c;
        }

        friend bool operator>=(const stack& lhs, const stack& rhs)
        {
            return lhs.c >= rhs.c;
        }
    };
}
