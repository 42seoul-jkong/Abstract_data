/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include <algorithm.hpp>
#include <iterator.hpp>
#include <iterator/_pointer_iterator.hpp>

#include <cstddef>
#include <memory>

#include <cassert>

namespace ft
{
    template <typename T, typename TAlloc = std::allocator<T> >
    class vector
    {
    public:
        typedef T value_type;
        typedef TAlloc allocator_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef ft::_pointer_iterator<pointer, vector> iterator;
        typedef ft::_pointer_iterator<const_pointer, vector> const_iterator;
        typedef ft::reverse_iterator<iterator> reverse_iterator;
        typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

    private:
        pointer start;
        size_type length;
        size_type count;
        allocator_type alloc;

    public:
        vector() {}
        explicit vector(const allocator_type& alloc)
            : alloc(alloc) {}
        explicit vector(size_type count, const value_type& value = value_type(), const allocator_type& alloc = allocator_type());
        template <class UIter>
        vector(UIter first, UIter last, const allocator_type& alloc = allocator_type());
        vector(const vector& that);
        ~vector();
        vector& operator=(const vector& that);
        void assign(size_type count, const value_type& value);
        template <class UIter>
        void assign(UIter first, UIter last);
        allocator_type get_allocator() const;

        reference at(size_type pos);
        const_reference at(size_type pos) const;
        reference operator[](size_type pos);
        const_reference operator[](size_type pos) const;
        reference front();
        const_reference front() const;
        reference back();
        const_reference back() const;
        pointer data();
        const_pointer data() const;

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
        reverse_iterator rbegin();
        const_reverse_iterator rbegin() const;
        reverse_iterator rend();
        const_reverse_iterator rend() const;

        bool empty() const;
        size_type size() const;
        size_type max_size() const;
        void reserve(size_type new_cap);
        size_type capacity() const;

        void clear();
        iterator insert(iterator pos, const value_type& value);
        void insert(iterator pos, size_type count, const value_type& value);
        template <class UIter>
        void insert(iterator pos, UIter first, UIter last);
        iterator erase(iterator pos);
        iterator erase(iterator first, iterator last);
        void push_back(const value_type& value);
        void pop_back();
        void resize(size_type count, value_type value = value_type());
        void swap(vector& that);

    public:
        friend bool operator==(const vector& lhs, const vector& rhs)
        {
            return lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin());
        }

        friend bool operator!=(const vector& lhs, const vector& rhs)
        {
            return !(lhs == rhs);
        }

        friend bool operator<(const vector& lhs, const vector& rhs)
        {
            return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
        }

        friend bool operator<=(const vector& lhs, const vector& rhs)
        {
            return !(rhs < lhs);
        }

        friend bool operator>(const vector& lhs, const vector& rhs)
        {
            return rhs < lhs;
        }

        friend bool operator>=(const vector& lhs, const vector& rhs)
        {
            return !(lhs < rhs);
        }
    };

    template <typename T, typename TAlloc>
    inline void swap(
        vector<T, TAlloc>& lhs,
        vector<T, TAlloc>& rhs)
    {
        lhs.swap(rhs);
    }
}
