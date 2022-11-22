/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

namespace ft
{
    template <typename T>
    struct is_iterator
    {
        typedef char (&yes)[1];
        typedef char (&no)[2];

        template <typename UIter>
        static yes f(typename UIter::iterator_category*);

        template <typename UAny>
        static no f(...);

        enum
        {
            value = sizeof((f<T>)(0)) == sizeof(yes)
        };
    };
}
