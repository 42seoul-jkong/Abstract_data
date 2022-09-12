/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

namespace ft
{
    template <class T, T val>
    struct integral_constant
    {
        typedef T value_type;
        typedef integral_constant<T, val> type;
        static const T value = val;

        operator T() const { return val; }
    };

    template <class T, T val>
    T const integral_constant<T, val>::value;

    typedef integral_constant<bool, true> true_type;
    typedef integral_constant<bool, false> false_type;
}
