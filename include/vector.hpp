/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "algorithm.hpp"
#include "iterator.hpp"
#include "iterator/_pointer_iterator.hpp"
#include "stdexcept.hpp"
#include "type_traits.hpp"

#include <cstddef>
#include <limits>

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
        typedef ft::_internal::_pointer_iterator<pointer, vector> iterator;
        typedef ft::_internal::_pointer_iterator<const_pointer, vector> const_iterator;
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

        template <typename UIter>
        // vector(UIter first, UIter last, const allocator_type& alloc = allocator_type())
        vector(typename ft::enable_if<ft::is_iterator<UIter>::value, UIter>::type first, UIter last, const allocator_type& alloc = allocator_type())
            : start(), length(), count(), alloc(alloc)
        {
            this->assign(first, last);
        }

        vector(const vector& that)
            : start(), length(that.length), count(that.count), alloc(that.alloc)
        {
            size_type count = this->capacity();
            pointer assign = this->alloc.allocate(count);
            try
            {
                vector::uninitialized_copy(that.begin(), that.end(), iterator(assign), this->alloc);
            }
            catch (...)
            {
                this->alloc.deallocate(assign, count);
                throw;
            }
            this->start = assign;
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
            if (this->start != pointer())
            {
                for (size_type i = this->length; i != size_type();)
                {
                    --i;
                    this->alloc.destroy(ft::addressof(this->start[i]));
                }
                this->alloc.deallocate(this->start, this->count);
            }
        }

    protected:
        struct vector_operation_count
        {
            size_type length;
            const value_type& value;

            vector_operation_count(size_type length, const value_type& value)
                : length(length), value(value) {}

            inline size_type count() const { return this->length; }
            inline void copy_n_head(size_type pivot, iterator dest) const { vector::fill_n(dest, pivot, this->value); }
            inline void copy_n_tail(size_type pivot, iterator dest) const { vector::fill_n(dest, this->count() - pivot, this->value); }
            inline void uninitialized_copy_n_head(size_type pivot, iterator dest, allocator_type& alloc) const { vector::uninitialized_fill_n(dest, pivot, this->value, alloc); }
            inline void uninitialized_copy_n_tail(size_type pivot, iterator dest, allocator_type& alloc) const { vector::uninitialized_fill_n(dest, this->count() - pivot, this->value, alloc); }
        };

        template <typename UIter>
        struct vector_operation_range
        {
            mutable UIter first;
            mutable UIter last;

            size_type length;

            vector_operation_range(UIter first, UIter last)
                : first(first), last(last)
            {
                this->length = ft::distance(first, last);
            }

            inline size_type count() const { return this->length; }
            inline void copy_n_head(size_type pivot, iterator dest) const { ft::copy(this->first, vector::advance(this->first, pivot), dest); }
            inline void copy_n_tail(size_type pivot, iterator dest) const { ft::copy(vector::advance(this->first, pivot), this->last, dest); }
            inline void uninitialized_copy_n_head(size_type pivot, iterator dest, allocator_type& alloc) const { vector::uninitialized_copy(this->first, vector::advance(this->first, pivot), dest, alloc); }
            inline void uninitialized_copy_n_tail(size_type pivot, iterator dest, allocator_type& alloc) const { vector::uninitialized_copy(vector::advance(this->first, pivot), this->last, dest, alloc); }
        };

        template <typename TOp>
        void assign_internal(const TOp& adaptor)
        {
            size_type count = this->check(adaptor.count(), "vector::assign");
            if (this->capacity() >= count)
            {
                size_type length = this->size();
                if (length < count)
                {
                    adaptor.copy_n_head(length, this->begin());
                    adaptor.uninitialized_copy_n_tail(length, vector::advance(this->begin(), length), this->alloc);
                }
                else
                {
                    adaptor.copy_n_head(count, this->begin());
                    this->destroy_tail_n(length - count);
                }
                this->length = count;
            }
            else
            {
                pointer assign = this->alloc.allocate(count);
                try
                {
                    adaptor.uninitialized_copy_n_head(count, iterator(assign), this->alloc);
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
        void assign(size_type count, const value_type& value)
        {
            this->assign_internal(vector_operation_count(count, value));
        }

        template <typename UIter>
        // void assign(UIter first, UIter last)
        typename ft::enable_if<ft::is_forward_iterator<UIter>::value, void>::type assign(UIter first, UIter last)
        {
            this->assign_internal(vector_operation_range<UIter>(first, last));
        }

        template <typename UIter>
        // void assign(UIter first, UIter last)
        typename ft::enable_if<!ft::is_forward_iterator<UIter>::value && ft::is_input_iterator<UIter>::value, void>::type assign(UIter first, UIter last)
        {
            this->clear();
            for (; first != last; ++first)
            {
                this->push_back(*first);
            }
        }

    public:
        allocator_type get_allocator() const { return this->alloc; }

    public:
        reference at(size_type pos)
        {
            if (!(pos < this->size()))
            {
                throw ft::out_of_range("vector::at");
            }
            return this->start[pos];
        }
        const_reference at(size_type pos) const
        {
            if (!(pos < this->size()))
            {
                throw ft::out_of_range("vector::at");
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
                throw ft::length_error("vector::reserve");
            }
            if (this->capacity() < new_cap)
            {
                pointer reserve = this->alloc.allocate(new_cap);
                try
                {
                    vector::uninitialized_copy(this->begin(), this->end(), iterator(reserve), this->alloc);
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
        inline size_type check(size_type length, const char* caller)
        {
            if (this->max_size() < length)
            {
                throw ft::length_error(caller);
            }
            return length;
        }

        inline size_type expand(size_type delta, const char* caller)
        {
            size_type length = this->size();
            if (this->max_size() - length < delta)
            {
                throw ft::length_error(caller);
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
        // static inline UIter advance(UIter it, size_type n)
        static inline typename ft::enable_if<ft::is_iterator<UIter>::value, UIter>::type advance(UIter it, difference_type n)
        {
            ft::advance(it, n);
            return it;
        }

        static inline void fill_n(iterator pos, size_type count, const value_type& value)
        {
            iterator it = pos;
            for (; count != 0; --count)
            {
                *it = value;
                ++it;
            }
        }

        static inline void uninitialized_fill_n(iterator pos, size_type count, const value_type& value, allocator_type& alloc)
        {
            iterator it = pos;
            try
            {
                for (; count != 0; --count)
                {
                    alloc.construct(ft::addressof(*it), value);
                    ++it;
                }
            }
            catch (...)
            {
                for (; it != pos;)
                {
                    --it;
                    alloc.destroy(ft::addressof(*it));
                }
                throw;
            }
        }

        template <typename UIter>
        // static inline void uninitialized_copy(UIter first, UIter last, iterator pos, allocator_type& alloc)
        static inline typename ft::enable_if<ft::is_iterator<UIter>::value, void>::type uninitialized_copy(UIter first, UIter last, iterator pos, allocator_type& alloc)
        {
            iterator it = pos;
            try
            {
                for (UIter val = first; val != last; ++val)
                {
                    alloc.construct(ft::addressof(*it), *val);
                    ++it;
                }
            }
            catch (...)
            {
                for (; it != pos;)
                {
                    --it;
                    alloc.destroy(ft::addressof(*it));
                }
                throw;
            }
        }

    protected:
        template <typename TOp>
        void insert_internal(iterator pos, const TOp& adaptor)
        {
            size_type count = adaptor.count();
            size_type index = ft::distance(this->begin(), pos);
            size_type len = this->size();
            size_type cap = this->capacity();
            size_type new_cap = this->expand(count, "vector::insert");
            if (cap == new_cap)
            {
                size_type difference = ft::distance(pos, this->end());
                if (len <= index + count)
                {
                    // [end, end + (count - difference))
                    adaptor.uninitialized_copy_n_tail(difference, this->end(), this->alloc);
                    this->length += count - difference;

                    // [pos, pos + difference) --(move)-> [(end + (count - difference)) == pos + count, (end + (count - difference)) + difference == pos + count + difference)
                    vector::uninitialized_copy(pos, vector::advance(pos, difference), this->end(), this->alloc);
                    this->length += difference;

                    // [pos, pos + difference)
                    adaptor.copy_n_head(difference, pos);
                }
                else
                {
                    // [end - count, end) --(move)-> [end, end + count)
                    vector::uninitialized_copy(vector::advance(this->end(), -count), this->end(), this->end(), this->alloc);
                    this->length += count;

                    // [pos, pos + (difference - count) == end - count) --(move reverse)-> (end, end - (difference - count) == pos + count]
                    ft::copy_backward(pos, vector::advance(pos, difference - count), vector::advance(pos, difference));

                    // [pos, pos + count)
                    adaptor.copy_n_head(count, pos);
                }
            }
            else
            {
                pointer insert = this->alloc.allocate(new_cap);
                try
                {
                    vector::uninitialized_copy(this->begin(), pos, iterator(insert), this->alloc);
                    try
                    {
                        adaptor.uninitialized_copy_n_head(count, vector::advance(iterator(insert), index), this->alloc);
                        try
                        {
                            vector::uninitialized_copy(pos, this->end(), vector::advance(iterator(insert), index + count), this->alloc);
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

    public:
        void clear()
        {
            this->destroy_tail_n(this->length);
        }

        iterator insert(iterator pos, const value_type& value)
        {
            size_type index = ft::distance(this->begin(), pos);
            this->insert(pos, size_type(1), value);
            return vector::advance(this->begin(), index);
        }

        void insert(iterator pos, size_type count, const value_type& value)
        {
            this->insert_internal(pos, vector_operation_count(count, value));
        }

        template <typename UIter>
        // void insert(iterator pos, UIter first, UIter last)
        typename ft::enable_if<ft::is_forward_iterator<UIter>::value, void>::type insert(iterator pos, UIter first, UIter last)
        {
            this->insert_internal(pos, vector_operation_range<UIter>(first, last));
        }

        template <typename UIter>
        // void insert(iterator pos, UIter first, UIter last)
        typename ft::enable_if<!ft::is_forward_iterator<UIter>::value && ft::is_input_iterator<UIter>::value, void>::type insert(iterator pos, UIter first, UIter last)
        {
            for (; first != last; ++first)
            {
                pos = this->insert(pos, *first);
                ++pos;
            }
        }

        iterator erase(iterator pos)
        {
            size_type count = 1;
            ft::copy(vector::advance(pos, count), this->end(), pos);
            this->destroy_tail_n(count);
            return pos;
        }

        iterator erase(iterator first, iterator last)
        {
            size_type count = ft::distance(first, last);
            ft::copy(last, this->end(), first);
            this->destroy_tail_n(count);
            return first;
        }

        void push_back(const value_type& value)
        {
            if (this->size() < this->capacity())
            {
                // fast way
                alloc.construct(ft::addressof(*this->end()), value);
                this->length++;
            }
            else
            {
                this->insert(this->end(), value);
            }
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
            ft::swap(this->start, that.start);
            ft::swap(this->length, that.length);
            ft::swap(this->count, that.count);

            ft::swap(this->alloc, that.alloc);
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

namespace std
{
    template <typename T, typename TAlloc>
    inline void swap(
        ft::vector<T, TAlloc>& lhs,
        ft::vector<T, TAlloc>& rhs)
    {
        ft::swap(lhs, rhs);
    }
}
