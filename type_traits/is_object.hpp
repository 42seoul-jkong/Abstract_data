/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "integral_constant.hpp"
#include "is_reference.hpp"
#include "is_void.hpp"

namespace ft
{
    template <typename T>
    struct is_object
        : public ft::integral_constant<
              bool,
              !ft::is_reference<T>::value &&
                  !ft::is_void<T>::value &&
                  !ft::is_function<T>::value>
    {
    };
}
