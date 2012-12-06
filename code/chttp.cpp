// Copyright (c) 2012, Andre Caron (andre.l.caron@gmail.com)
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//  * Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

/*!
 * @file
 * @brief C++ API implementation.
 */

#include "chttp.hpp"

namespace http {

    Head::Head (std::size_t size)
    {
        if (::http_head_init(&myBackend, size) == 0) {
            throw (std::bad_alloc());
        }
    }

    Head::~Head ()
    {
        ::http_head_kill(&myBackend);
    }

    ::http_head& Head::backend ()
    {
        return (myBackend);
    }

    const ::http_head& Head::backend () const
    {
        return (myBackend);
    }

    bool Head::push (const std::string& field, const std::string& value)
    {
        return (::http_head_push(&myBackend, field.c_str(),
                                             value.c_str()) == 0);
    }

    std::string Head::find (const std::string& field) const
    {
        return (::http_head_find(&myBackend, field.c_str()));
    }

    Cursor::Cursor (const Head& head)
    {
        ::http_cursor_init(&myBackend, &head.backend());
    }

    bool Cursor::next ()
    {
        return (::http_cursor_next(&myBackend) != 0);
    }

    std::string Cursor::field () const
    {
        return (myBackend.field);
    }

    std::string Cursor::value () const
    {
        return (myBackend.value);
    }

}
