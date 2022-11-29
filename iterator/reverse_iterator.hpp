/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "iterator.hpp"
#include "iterator_traits.hpp"

#include <memory.hpp>

namespace ft
{
    template <typename TIter>
    class reverse_iterator
        : public iterator<typename ft::iterator_traits<TIter>::iterator_category,
                          typename ft::iterator_traits<TIter>::value_type,
                          typename ft::iterator_traits<TIter>::difference_type,
                          typename ft::iterator_traits<TIter>::pointer,
                          typename ft::iterator_traits<TIter>::reference>
    {
    protected:
        typedef ft::iterator_traits<TIter> _Traits;

    public:
        typedef TIter iterator_type;
        typedef typename _Traits::iterator_category iterator_category;
        typedef typename _Traits::value_type value_type;
        typedef typename _Traits::difference_type difference_type;
        typedef typename _Traits::pointer pointer;
        typedef typename _Traits::reference reference;

    protected:
        TIter current;

    public:
        reverse_iterator() : current() {}
        explicit reverse_iterator(iterator_type current) : current(current) {}
        template <typename U>
        reverse_iterator(const reverse_iterator<U>& that) : current(that.base()) {}

        iterator_type base() const { return this->current; }

        reference operator*() const
        {
            TIter tmp = this->current;
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
            --this->current;
            return *this;
        }

        reverse_iterator& operator--()
        {
            ++this->current;
            return *this;
        }

        reverse_iterator operator++(int)
        {
            reverse_iterator tmp = *this;
            --this->current;
            return tmp;
        }

        reverse_iterator operator--(int)
        {
            reverse_iterator tmp = *this;
            ++this->current;
            return tmp;
        }

        reverse_iterator operator+(difference_type n) const
        {
            return reverse_iterator(this->current - n);
        }

        reverse_iterator operator-(difference_type n) const
        {
            return reverse_iterator(this->current + n);
        }

        reverse_iterator& operator+=(difference_type n)
        {
            this->current -= n;
            return *this;
        }

        reverse_iterator& operator-=(difference_type n)
        {
            this->current += n;
            return *this;
        }
    };

    template <typename TIterFirst, typename TIterSecond>
    inline bool operator==(
        const reverse_iterator<TIterFirst>& lhs,
        const reverse_iterator<TIterSecond>& rhs)
    {
        return lhs.base() == rhs.base();
    }

    template <typename TIterFirst, typename TIterSecond>
    inline bool operator!=(
        const reverse_iterator<TIterFirst>& lhs,
        const reverse_iterator<TIterSecond>& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename TIterFirst, typename TIterSecond>
    inline bool operator<(
        const reverse_iterator<TIterFirst>& lhs,
        const reverse_iterator<TIterSecond>& rhs)
    {
        return rhs.base() < lhs.base();
    }

    template <typename TIterFirst, typename TIterSecond>
    inline bool operator<=(
        const reverse_iterator<TIterFirst>& lhs,
        const reverse_iterator<TIterSecond>& rhs)
    {
        return !(rhs < lhs);
    }

    template <typename TIterFirst, typename TIterSecond>
    inline bool operator>(
        const reverse_iterator<TIterFirst>& lhs,
        const reverse_iterator<TIterSecond>& rhs)
    {
        return rhs < lhs;
    }

    template <typename TIterFirst, typename TIterSecond>
    inline bool operator>=(
        const reverse_iterator<TIterFirst>& lhs,
        const reverse_iterator<TIterSecond>& rhs)
    {
        return !(lhs < rhs);
    }

    template <typename TIter>
    inline reverse_iterator<TIter> operator+(
        typename reverse_iterator<TIter>::difference_type n,
        const reverse_iterator<TIter>& it)
    {
        return reverse_iterator<TIter>(it.base() - n);
    }

    template <typename TIter>
    inline typename reverse_iterator<TIter>::difference_type operator-(
        const reverse_iterator<TIter>& lhs,
        const reverse_iterator<TIter>& rhs)
    {
        return rhs.base() - lhs.base();
    }

    template <typename TIterFirst, typename TIterSecond>
    inline typename reverse_iterator<TIterFirst>::difference_type operator-(
        const reverse_iterator<TIterFirst>& lhs,
        const reverse_iterator<TIterSecond>& rhs)
    {
        return rhs.base() - lhs.base();
    }
}
