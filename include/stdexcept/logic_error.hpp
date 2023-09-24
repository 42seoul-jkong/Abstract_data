/* Any copyright is dedicated to the Public Domain.
 * https://creativecommons.org/publicdomain/zero/1.0/ */

#pragma once

#include "../exception.hpp"

#include <string>

namespace ft
{
    class logic_error : public ft::exception
    {
    private:
        // FIXME: exception unsafe
        std::string what_arg;

    public:
        logic_error() throw() {}
        explicit logic_error(const std::string& what_arg) throw() : what_arg(what_arg.c_str()) {}
        explicit logic_error(const char* what_arg) throw() : what_arg(what_arg) {}
        logic_error(const logic_error& that) throw()
            : ft::exception(that), what_arg(that.what_arg) {}
        logic_error& operator=(const logic_error& that) throw()
        {
            this->what_arg = that.what_arg;
            return *this;
        }
        virtual ~logic_error() throw() {}
        virtual const char* what() const throw() { return this->what_arg.c_str(); }
    };

#define DEFINE_LOGIC_ERROR(_name)                                               \
    class _name : public ft::logic_error                                        \
    {                                                                           \
    public:                                                                     \
        explicit _name(const std::string& what_arg) : ft::logic_error(what_arg) \
        {                                                                       \
        }                                                                       \
        explicit _name(const char* what_arg) : ft::logic_error(what_arg)        \
        {                                                                       \
        }                                                                       \
        virtual ~_name() throw()                                                \
        {                                                                       \
        }                                                                       \
    }

    DEFINE_LOGIC_ERROR(invalid_argument);
    DEFINE_LOGIC_ERROR(domain_error);
    DEFINE_LOGIC_ERROR(length_error);
    DEFINE_LOGIC_ERROR(out_of_range);
}
