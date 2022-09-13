/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include <cstddef>

namespace ft
{
    // LegacyInputIterator
    struct input_iterator_tag
    {
    };

    // LegacyOutputIterator
    struct output_iterator_tag
    {
    };

    // LegacyForwardIterator
    struct forward_iterator_tag : public input_iterator_tag
    {
    };

    // LegacyBidirectionalIterator
    struct bidirectional_iterator_tag : public forward_iterator_tag
    {
    };

    // LegacyRandomAccessIterator
    struct random_access_iterator_tag : public bidirectional_iterator_tag
    {
    };

    template <typename TIter>
    struct iterator_traits
    {
        typedef typename TIter::difference_type difference_type;
        typedef typename TIter::value_type value_type;
        typedef typename TIter::pointer pointer;
        typedef typename TIter::reference reference;
        typedef typename TIter::iterator_category iterator_category;
    };

    // Pointer Specialization
    template <typename T>
    struct iterator_traits<T*>
    {
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef random_access_iterator_tag iterator_category;
    };

    // Const-Pointer Specialization
    template <typename T>
    struct iterator_traits<const T*>
    {
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T* pointer;
        typedef const T& reference;
        typedef random_access_iterator_tag iterator_category;
    };
}
