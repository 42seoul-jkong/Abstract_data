/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

namespace ft
{
    template <class T, T Value>
    struct integral_constant
    {
        typedef T value_type;
        typedef integral_constant<T, Value> type;
        static const T value = Value;

        operator T() const { return Value; }
    };

    template <class T, T Value>
    T const integral_constant<T, Value>::value;

    typedef integral_constant<bool, true> true_type;
    typedef integral_constant<bool, false> false_type;
}
