/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

namespace ft
{
    template <class TIterFirst, class TIterSecond>
    inline bool
    equal(
        TIterFirst first1, TIterFirst last1,
        TIterSecond first2)
    {
        for (; first1 != last1; ++first1, ++first2)
        {
            if (!(*first1 == *first2))
            {
                return false;
            }
        }
        return true;
    }

    template <class TIterFirst, class TIterSecond, class BinaryPredicate>
    inline bool
    equal(
        TIterFirst first1, TIterFirst last1,
        TIterSecond first2,
        BinaryPredicate p)
    {
        for (; first1 != last1; ++first1, ++first2)
        {
            if (!p(*first1, *first2))
            {
                return false;
            }
        }
        return true;
    }
}
