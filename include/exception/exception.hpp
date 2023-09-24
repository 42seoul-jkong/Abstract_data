/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

namespace ft
{
    class exception
    {
    public:
        exception() throw() {}
        exception(const exception&) throw() {}
        exception& operator=(const exception&) throw() { return *this; }
        virtual ~exception() throw() {}
        virtual const char* what() const throw() { return "exception"; }
    };
}
