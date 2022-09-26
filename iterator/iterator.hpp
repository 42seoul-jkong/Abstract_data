/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include <cstddef>

namespace ft
{
    template <
        typename TIterCat,
        typename T,
        typename TDistance = std::ptrdiff_t,
        typename TPointer = T*,
        typename TReference = T&>
    struct iterator
    {
        typedef TIterCat iterator_category;
        typedef T value_type;
        typedef TDistance difference_type;
        typedef TPointer pointer;
        typedef TReference reference;
    };
}
