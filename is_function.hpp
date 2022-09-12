/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "integral_constant.hpp"
#include "is_const.hpp"
#include "is_reference.hpp"

namespace ft
{
    template <class T>
    struct is_function
        : ft::integral_constant<
              bool,
              !ft::is_const<const T>::value &&
                  !ft::is_reference<T>::value>
    {
    };
}
