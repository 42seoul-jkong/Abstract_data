/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "../iterator.hpp"

namespace ft
{
    // TODO: if is_trivially_copyable then use std::memmove
    template <typename TIter, typename UIter>
    inline UIter copy(TIter first, TIter last, UIter pos)
    {
        UIter it = pos;
        for (TIter val = first; val != last; ++val)
        {
            *it = *val;
            ++it;
        }
        return it;
    }

    template <typename TIter, typename UIter>
    inline UIter copy_backward(TIter first, TIter last, UIter pos)
    {
        UIter it = pos;
        for (TIter val = last; val != first;)
        {
            --val;
            --it;
            *it = *val;
        }
        return it;
    }
}
