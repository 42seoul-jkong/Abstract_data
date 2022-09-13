/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include <cstddef>
#include <memory>

#include <iterator.hpp>
#include <iterator/_pointer_iterator.hpp>

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
        pointer _start;
        size_type _length;
        size_type _capacity;

    public:
        vector();
        explicit vector(const TAlloc& alloc);
        explicit vector(size_type count, const T& value = T(), const TAlloc& alloc = TAlloc());
        template <class UIter>
        vector(UIter first, UIter last, const TAlloc& alloc = TAlloc());
        vector(const vector& that);
        ~vector();
        vector& operator=(const vector& that);
        void assign(size_type count, const T& value);
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
        iterator insert(iterator pos, const T& value);
        void insert(iterator pos, size_type count, const T& value);
        template <class UIter>
        void insert(iterator pos, UIter first, UIter last);
        iterator erase(iterator pos);
        iterator erase(iterator first, iterator last);
        void push_back(const T& value);
        void pop_back();
        void resize(size_type count, T value = T());
        void swap(vector& that);
    };

    template <class T, class TAlloc>
    inline bool operator==(
        const std::vector<T, TAlloc>& lhs,
        const std::vector<T, TAlloc>& rhs);

    template <class T, class TAlloc>
    inline bool operator!=(
        const std::vector<T, TAlloc>& lhs,
        const std::vector<T, TAlloc>& rhs);

    template <class T, class TAlloc>
    inline bool operator<(
        const std::vector<T, TAlloc>& lhs,
        const std::vector<T, TAlloc>& rhs);

    template <class T, class TAlloc>
    inline bool operator<=(
        const std::vector<T, TAlloc>& lhs,
        const std::vector<T, TAlloc>& rhs);

    template <class T, class TAlloc>
    inline bool operator>(
        const std::vector<T, TAlloc>& lhs,
        const std::vector<T, TAlloc>& rhs);

    template <class T, class TAlloc>
    inline bool operator>=(
        const std::vector<T, TAlloc>& lhs,
        const std::vector<T, TAlloc>& rhs);

    template <class T, class TAlloc>
    inline void swap(
        std::vector<T, TAlloc>& lhs,
        std::vector<T, TAlloc>& rhs);
}
