/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "addressof.hpp"
#include "iterator.hpp"
#include "iterator_traits.hpp"

namespace ft
{
    template <class TIterator>
    class reverse_iterator
        : public iterator<typename iterator_traits<TIterator>::iterator_category,
                          typename iterator_traits<TIterator>::value_type,
                          typename iterator_traits<TIterator>::difference_type,
                          typename iterator_traits<TIterator>::pointer,
                          typename iterator_traits<TIterator>::reference>
    {
    protected:
        TIterator current;

        typedef iterator_traits<TIterator> _Traits;

    public:
        typedef TIterator iterator_type;
        typedef typename _Traits::difference_type iterator_category;
        typedef typename _Traits::difference_type value_type;
        typedef typename _Traits::difference_type difference_type;
        typedef typename _Traits::pointer pointer;
        typedef typename _Traits::reference reference;

        reverse_iterator() : current() {}
        explicit reverse_iterator(iterator_type current) : current(current) {}
        template <class U>
        reverse_iterator(const reverse_iterator<U>& other) : current(other.base()) {}

        iterator_type base() const { return current; }

        reference operator*() const
        {
            TIterator tmp = current;
            return *--tmp;
        }

        pointer operator->() const
        {
            return ft::addressof(*(*this));
        }

        reference operator[](difference_type n) const
        {
            return *(*this + n);
        }

        reverse_iterator& operator++()
        {
            --current;
            return *this;
        }

        reverse_iterator& operator--()
        {
            ++current;
            return *this;
        }

        reverse_iterator operator++(int)
        {
            reverse_iterator tmp = *this;
            --current;
            return tmp;
        }

        reverse_iterator operator--(int)
        {
            reverse_iterator tmp = *this;
            ++current;
            return tmp;
        }

        reverse_iterator operator+(difference_type n) const
        {
            return reverse_iterator(current - n);
        }

        reverse_iterator operator-(difference_type n) const
        {
            return reverse_iterator(current + n);
        }

        reverse_iterator& operator+=(difference_type n) const
        {
            current -= n;
            return *this;
        }

        reverse_iterator& operator-=(difference_type n) const
        {
            current += n;
            return *this;
        }
    };

    template <typename TIteratorFirst, typename TIteratorSecond>
    inline bool operator==(
        const reverse_iterator<TIteratorFirst>& lhs,
        const reverse_iterator<TIteratorSecond>& rhs)
    {
        return lhs.base() == rhs.base();
    }

    template <typename TIteratorFirst, typename TIteratorSecond>
    inline bool operator!=(
        const reverse_iterator<TIteratorFirst>& lhs,
        const reverse_iterator<TIteratorSecond>& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename TIteratorFirst, typename TIteratorSecond>
    inline bool operator<(
        const reverse_iterator<TIteratorFirst>& lhs,
        const reverse_iterator<TIteratorSecond>& rhs)
    {
        return rhs.base() < lhs.base();
    }

    template <typename TIteratorFirst, typename TIteratorSecond>
    inline bool operator<=(
        const reverse_iterator<TIteratorFirst>& lhs,
        const reverse_iterator<TIteratorSecond>& rhs)
    {
        return !(rhs < lhs);
    }

    template <typename TIteratorFirst, typename TIteratorSecond>
    inline bool operator>(
        const reverse_iterator<TIteratorFirst>& lhs,
        const reverse_iterator<TIteratorSecond>& rhs)
    {
        return rhs < lhs;
    }

    template <typename TIteratorFirst, typename TIteratorSecond>
    inline bool operator>=(
        const reverse_iterator<TIteratorFirst>& lhs,
        const reverse_iterator<TIteratorSecond>& rhs)
    {
        return !(lhs < rhs);
    }

    template <typename TIterator>
    inline reverse_iterator<TIterator> operator+(
        typename reverse_iterator<TIterator>::difference_type n,
        const reverse_iterator<TIterator>& it)
    {
        return reverse_iterator<TIterator>(it.base() - n);
    }

    template <typename TIterator>
    inline typename reverse_iterator<TIterator>::difference_type operator-(
        const reverse_iterator<TIterator>& lhs,
        const reverse_iterator<TIterator>& rhs)
    {
        return rhs.base() - lhs.base();
    }
}
