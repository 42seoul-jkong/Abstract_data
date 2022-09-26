/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include <type_traits.hpp>

namespace ft
{
    template <typename T>
    typename ft::enable_if<ft::is_object<T>::value, T*>::type addressof(T& arg) throw()
    {
        return reinterpret_cast<T*>(
            &const_cast<char&>(
                reinterpret_cast<const volatile char&>(arg)));
    }

    template <typename T>
    typename ft::enable_if<!ft::is_object<T>::value, T*>::type addressof(T& arg) throw()
    {
        return &arg;
    }
}
