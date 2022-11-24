/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include <algorithm.hpp>
#include <iterator.hpp>
#include <iterator/_pointer_iterator.hpp>
#include <type_traits.hpp>

#include <algorithm>
#include <cstddef>
#include <exception>
#include <iterator>
#include <limits>
#include <memory>

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
        vector()
            : start(), length(), count(), alloc() {}

        explicit vector(const allocator_type& alloc)
            : start(), length(), count(), alloc(alloc) {}

        explicit vector(size_type count, const value_type& value = value_type(), const allocator_type& alloc = allocator_type())
            : start(), length(), count(), alloc(alloc)
        {
            this->assign(count, value);
        }

        template <class UIter>
        // vector(UIter first, UIter last, const allocator_type& alloc = allocator_type())
        vector(typename ft::enable_if<ft::is_iterator<UIter>::value, UIter>::type first, UIter last, const allocator_type& alloc = allocator_type())
            : start(), length(), count(), alloc(alloc)
        {
            this->assign(first, last);
        }

        vector(const vector& that)
            : start(), length(that.length), count(that.count), alloc(that.alloc)
        {
            this->start = this->alloc.allocate(this->capacity());
            std::uninitialized_copy(that.begin(), that.end(), this->begin());
        }

        ~vector()
        {
            this->destruct();
        }

        vector& operator=(const vector& that)
        {
            if (this != &that)
            {
                vector temp = that;
                this->swap(temp);
            }
            return *this;
        }

    protected:
        void destroy_tail_n(size_type count)
        {
            for (size_type i = size_type(); i < count; i++)
            {
                --this->length;
                this->alloc.destroy(ft::addressof(this->start[this->length]));
            }
        }

        void destruct()
        {
            for (size_type i = this->length; i != size_type();)
            {
                --i;
                this->alloc.destroy(ft::addressof(this->start[i]));
            }
            this->alloc.deallocate(this->start, this->count);
        }

    public:
        void assign(size_type count, const value_type& value)
        {
            if (this->capacity() >= count)
            {
                size_type length = this->size();
                if (length < count)
                {
                    std::fill_n(this->begin(), length, value);
                    std::uninitialized_fill_n(vector::advance(this->begin(), length), count - length, value);
                }
                else
                {
                    std::fill_n(this->begin(), count, value);
                    this->destroy_tail_n(length - count);
                }
                this->length = count;
            }
            else
            {
                pointer assign = this->alloc.allocate(count);
                try
                {
                    std::uninitialized_fill_n(iterator(assign), count, value);
                }
                catch (...)
                {
                    this->alloc.deallocate(assign, count);
                    throw;
                }
                this->destruct();
                this->start = assign;
                this->length = count;
                this->count = count;
            }
        }

        template <class UIter>
        // void assign(UIter first, UIter last)
        typename ft::enable_if<ft::is_iterator<UIter>::value, void>::type assign(UIter first, UIter last)
        {
            size_type count = std::distance(first, last);
            if (this->capacity() >= count)
            {
                size_type length = this->size();
                if (length < count)
                {
                    std::copy(first, vector::advance(first, length), this->begin());
                    std::uninitialized_copy(vector::advance(first, length), last, vector::advance(this->begin(), length));
                }
                else
                {
                    std::copy(first, last, this->begin());
                    this->destroy_tail_n(length - count);
                }
                this->length = count;
            }
            else
            {
                pointer assign = this->alloc.allocate(count);
                try
                {
                    std::uninitialized_copy(first, last, iterator(assign));
                }
                catch (...)
                {
                    this->alloc.deallocate(assign, count);
                    throw;
                }
                this->destruct();
                this->start = assign;
                this->length = count;
                this->count = count;
            }
        }

    public:
        allocator_type get_allocator() const { return this->alloc; }

    public:
        reference at(size_type pos)
        {
            if (!(pos < this->size()))
            {
                throw std::out_of_range("vector::at");
            }
            return this->start[pos];
        }
        const_reference at(size_type pos) const
        {
            if (!(pos < this->size()))
            {
                throw std::out_of_range("vector::at");
            }
            return this->start[pos];
        }

        reference operator[](size_type pos) { return this->start[pos]; }
        const_reference operator[](size_type pos) const { return this->start[pos]; }

        reference front() { return *this->start; }
        const_reference front() const { return *this->start; }
        reference back() { return this->start[this->length - 1]; }
        const_reference back() const { return this->start[this->length - 1]; }
        pointer data() { return this->start; }
        const_pointer data() const { return this->start; }

    public:
        iterator begin() { return iterator(this->start); }
        const_iterator begin() const { return const_iterator(this->start); }
        iterator end() { return iterator(ft::addressof(this->start[this->length])); }
        const_iterator end() const { return const_iterator(ft::addressof(this->start[this->length])); }
        reverse_iterator rbegin() { return reverse_iterator(this->end()); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(this->end()); }
        reverse_iterator rend() { return reverse_iterator(this->begin()); }
        const_reverse_iterator rend() const { return const_reverse_iterator(this->begin()); }

    public:
        bool empty() const { return this->size() == size_type(); }
        size_type size() const { return this->length; }
        size_type max_size() const { return std::numeric_limits<size_type>::max() / sizeof(value_type); }

        void reserve(size_type new_cap)
        {
            if (new_cap > this->max_size())
            {
                throw std::length_error("vector::reserve");
            }
            if (this->capacity() < new_cap)
            {
                pointer reserve = this->alloc.allocate(new_cap);
                try
                {
                    std::uninitialized_copy(this->begin(), this->end(), iterator(reserve));
                }
                catch (...)
                {
                    this->alloc.deallocate(reserve, new_cap);
                    throw;
                }
                this->destruct();
                this->start = reserve;
                this->count = new_cap;
            }
        }

        size_type capacity() const { return this->count; }

    protected:
        size_type expand(size_type delta, const char* caller)
        {
            size_type length = this->size();
            if (this->max_size() - length < delta)
            {
                throw std::length_error(caller);
            }
            size_type count = this->capacity();
            if (length + delta > count)
            {
                if (delta <= length)
                {
                    // same, length *= 2;
                    length <<= 1;
                }
                else
                {
                    length += delta;
                }
                count = length;
            }
            return count;
        }

    private:
        template <typename UIter>
        // static UIter advance(UIter it, size_type n)
        static typename ft::enable_if<ft::is_iterator<UIter>::value, UIter>::type advance(UIter it, difference_type n)
        {
            std::advance(it, n);
            return it;
        }

    public:
        void clear()
        {
            this->destroy_tail_n(this->length);
        }

        iterator insert(iterator pos, const value_type& value)
        {
            size_type index = std::distance(this->begin(), pos);
            this->insert(pos, 1, value);
            return vector::advance(this->begin(), index);
        }

        void insert(iterator pos, size_type count, const value_type& value)
        {
            size_type index = std::distance(this->begin(), pos);
            size_type len = this->size();
            size_type cap = this->capacity();
            size_type new_cap = this->expand(count, "vector::insert");
            if (cap == new_cap)
            {
                size_type difference = std::distance(pos, this->end());
                if (len <= index + count)
                {
                    std::uninitialized_fill_n(this->end(), count - difference, value);
                    this->length += count - difference;
                    std::uninitialized_copy(pos, vector::advance(pos, difference), this->end());
                    this->length += difference;
                    std::fill_n(pos, difference, value);
                }
                else
                {
                    std::uninitialized_copy(vector::advance(this->end(), -count), this->end(), this->end());
                    this->length += count;
                    std::copy_backward(pos, vector::advance(pos, difference - count), vector::advance(pos, difference));
                    std::fill_n(pos, count, value);
                }
            }
            else
            {
                pointer insert = this->alloc.allocate(new_cap);
                try
                {
                    std::uninitialized_copy(this->begin(), pos, iterator(insert));
                    try
                    {
                        std::uninitialized_fill_n(vector::advance(iterator(insert), index), count, value);
                        try
                        {
                            std::uninitialized_copy(pos, this->end(), vector::advance(iterator(insert), index + count));
                        }
                        catch (...)
                        {
                            for (size_type i = index + count; i != index;)
                            {
                                --i;
                                this->alloc.destroy(ft::addressof(insert[i]));
                            }
                            throw;
                        }
                    }
                    catch (...)
                    {
                        for (size_type i = index; i != size_type();)
                        {
                            --i;
                            this->alloc.destroy(ft::addressof(insert[i]));
                        }
                        throw;
                    }
                }
                catch (...)
                {
                    this->alloc.deallocate(insert, new_cap);
                    throw;
                }
                this->destruct();
                this->start = insert;
                this->length += count;
                this->count = new_cap;
            }
        }

        template <class UIter>
        // void insert(iterator pos, UIter first, UIter last)
        typename ft::enable_if<ft::is_iterator<UIter>::value, void>::type insert(iterator pos, UIter first, UIter last)
        {
            size_type count = std::distance(first, last);
            size_type index = std::distance(this->begin(), pos);
            size_type len = this->size();
            size_type cap = this->capacity();
            size_type new_cap = this->expand(count, "vector::insert");
            if (cap == new_cap)
            {
                size_type difference = std::distance(pos, this->end());
                if (len <= index + count)
                {
                    std::uninitialized_copy(vector::advance(last, difference - count), last, this->end());
                    this->length += count - difference;
                    std::uninitialized_copy(pos, vector::advance(pos, difference), this->end());
                    this->length += difference;
                    std::copy(first, vector::advance(first, count - difference), pos);
                }
                else
                {
                    std::uninitialized_copy(vector::advance(this->end(), -count), this->end(), this->end());
                    this->length += count;
                    std::copy_backward(pos, vector::advance(pos, difference - count), vector::advance(pos, difference));
                    std::copy(first, last, pos);
                }
            }
            else
            {
                pointer insert = this->alloc.allocate(new_cap);
                try
                {
                    std::uninitialized_copy(this->begin(), pos, iterator(insert));
                    try
                    {
                        std::uninitialized_copy(first, last, vector::advance(iterator(insert), index));
                        try
                        {
                            std::uninitialized_copy(pos, this->end(), vector::advance(iterator(insert), index + count));
                        }
                        catch (...)
                        {
                            for (size_type i = index + count; i != index;)
                            {
                                --i;
                                this->alloc.destroy(ft::addressof(insert[i]));
                            }
                            throw;
                        }
                    }
                    catch (...)
                    {
                        for (size_type i = index; i != size_type();)
                        {
                            --i;
                            this->alloc.destroy(ft::addressof(insert[i]));
                        }
                        throw;
                    }
                }
                catch (...)
                {
                    this->alloc.deallocate(insert, new_cap);
                    throw;
                }
                this->destruct();
                this->start = insert;
                this->length += count;
                this->count = new_cap;
            }
        }

        iterator erase(iterator pos)
        {
            size_type count = 1;
            std::copy(vector::advance(pos, count), this->end(), pos);
            this->destroy_tail_n(count);
            return pos;
        }

        iterator erase(iterator first, iterator last)
        {
            size_type count = std::distance(first, last);
            std::copy(last, this->end(), first);
            this->destroy_tail_n(count);
            return first;
        }

        void push_back(const value_type& value)
        {
            this->insert(this->end(), value);
        }

        void pop_back()
        {
            this->erase(vector::advance(this->end(), -1));
        }

        void resize(size_type count, value_type value = value_type())
        {
            size_type size = this->size();
            if (count > size)
            {
                this->insert(this->end(), count - size, value);
            }
            else if (count < size)
            {
                this->erase(vector::advance(this->begin(), count), this->end());
            }
        }

        void swap(vector& that)
        {
            std::swap(this->start, that.start);
            std::swap(this->length, that.length);
            std::swap(this->count, that.count);

            std::swap(this->alloc, that.alloc);
        }

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
