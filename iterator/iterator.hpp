/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include <cstddef>

namespace ft
{
    template <
        class Category,
        class T,
        class TDiff = std::ptrdiff_t,
        class TPointer = T*,
        class TReference = T&>
    struct iterator
    {
        typedef Category iterator_category;
        typedef T value_type;
        typedef TDiff difference_type;
        typedef TPointer pointer;
        typedef TReference reference;
    };
}
