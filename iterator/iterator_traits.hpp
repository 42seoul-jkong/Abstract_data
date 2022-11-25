/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include <type_traits.hpp>

#include <cstddef>
#include <iterator>

namespace ft
{
    template <typename TIter, typename = void>
    struct iterator_traits
    {
    };

    template <typename TIter>
    struct iterator_traits<TIter, typename ft::void_t<typename TIter::iterator_category>::type>
    {
        typedef typename TIter::difference_type difference_type;
        typedef typename TIter::value_type value_type;
        typedef typename TIter::pointer pointer;
        typedef typename TIter::reference reference;
        typedef typename TIter::iterator_category iterator_category;
    };

    // Pointer Specialization
    template <typename T>
    struct iterator_traits<T*, void>
    {
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef std::random_access_iterator_tag iterator_category;
    };

    // Const-Pointer Specialization
    template <typename T>
    struct iterator_traits<const T*, void>
    {
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T* pointer;
        typedef const T& reference;
        typedef std::random_access_iterator_tag iterator_category;
    };
}
