/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "iterator_tags.hpp"

#include "../type_traits.hpp"

#include <cstddef>

namespace ft
{
    namespace _internal
    {
        template <typename T, typename = void>
        struct iterator_traits_base
        {
        };

        template <typename TIter>
        struct iterator_traits_base<TIter, typename ft::make_void<typename TIter::iterator_category>::type>
        {
            typedef typename TIter::difference_type difference_type;
            typedef typename TIter::value_type value_type;
            typedef typename TIter::pointer pointer;
            typedef typename TIter::reference reference;
            typedef typename TIter::iterator_category iterator_category;
        };
    }

    template <typename T>
    struct iterator_traits : _internal::iterator_traits_base<T>
    {
    };

    // Pointer Specialization
    template <typename T>
    struct iterator_traits<T*>
    {
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef ft::random_access_iterator_tag iterator_category;
    };

    // Const-Pointer Specialization
    template <typename T>
    struct iterator_traits<const T*>
    {
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T* pointer;
        typedef const T& reference;
        typedef ft::random_access_iterator_tag iterator_category;
    };
}
