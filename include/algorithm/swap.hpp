/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include <cstddef>

namespace ft
{
    template <typename T>
    inline void
    swap(
        T& a, T& b)
    {
        T x(a);
        a = b;
        b = x;
    }

    template <typename T, std::size_t N>
    inline void
    swap(
        T (&a)[N], T (&b)[N])
    {
        for (std::size_t i = 0; i < N; i++)
        {
            swap(a[i], b[i]);
        }
    }
}
