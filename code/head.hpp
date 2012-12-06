#ifndef _chttp_head_hpp__
#define _chttp_head_hpp__

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
 * @brief C++ API definition.
 */

#include "head.h"
#include <cstddef>
#include <memory>
#include <string>

/*!
 * @brief C++ wrappers for the `chttp` library.
 */
namespace http {

    /*!
     * @brief Buffer for HTTP headers.
     *
     * @see http_head
     */
    class Head
    {
        /* data. */
    private:
        ::http_head myBackend;

        /* construction. */
    public:
        /*!
         * @brief Create an empty buffer with a capacity of @a size bytes.
         * @param size Buffer capacity (fixed).
         * @exception std::bad_alloc Could not acquire @a size bytes of memory.
         */
        Head (std::size_t size);

        /*!
         * @brief Release memory acquired for buffering.
         */
        ~Head ();

        /* methods. */
    public:
        /*!
         * @internal
         * @brief Access the native representation.
         * @return The C structure that backs the object.
         */
        ::http_head& backend ();

        /*!
         * @internal
         * @brief Access the native representation.
         * @return The C structure that backs the object.
         */
        const ::http_head& backend () const;

        /*!
        * @brief Append an HTTP header to the buffer.
        * @param field HTTP header name.
        * @param value HTTP header data.
        * @return @c false on failure (e.g. attempted to exceed the buffer
        *  capacity), else @c true.
         */
        bool push (const std::string& field, const std::string& value);

        /*!
        * @brief Search for an HTTP header by name.
        * @param field The name of the HTTP header to look for.
        * @return @c An empty (zero-length) string if the header was not found,
        *  else a string containing the HTTP header data.
        *
        * This method is meant for localized processing of specific HTTP
        * headers (e.g. the content length).  Since it always performs a linear
        * search from the start of the buffer, it is usually more efficient to
        * use a @c Cursor to iterate over all headers.
        *
        * @see Cursor
        */
        std::string find (const std::string& field) const;
    };

    /*!
     * @brief Iterator for HTTP headers.
     *
     * @note The class uses a Java iterator style rather than the pointer
     *  syntax from the stnadard library.  This has been done to map the C++
     *  API very closely onto the native C API.
     *
     * Recommended use:
     * @code
     *  http::Head head(4*1024);
     *  // ...
     *  // Do something that buffers HTTP headers.
     *  // ...
     *  // OK, we're ready to process HTTP headers.
     *  http::Cursor cursor(head);
     *  while (cursor.next())
     *  {
     *    if (cursor.field() == "Content-Length") {
     *      // ...
     *    }
     *  }
     * @endcode
     *
     * @see Head::find
     */
    class Cursor
    {
        /* data. */
    private:
        ::http_cursor myBackend;

        /* construction. */
    public:
        /*!
         * @brief Prepare for iteration over @a head.
         * @param head HTTP headers over which to iterate.
         *
         * @warning You should call @c next() right after this.
         */
        explicit Cursor (const Head& head);

        /* methods. */
    public:
        /*!
         * @brief Locates the next HTTP header.
         * @return @c false if we had already reached the end of the headers,
         *  else @c true.
         * @post You may call @c field() and @c value() to recover the HTTP
         *  header name and data.
         */
        bool next ();

        /*!
         * @brief Obtains the current HTTP header's name.
         * @return The current HTTP header's name.
         * @pre @c next() just returned @c true.
         *
         * @see next()
         * @see value()
         */
        std::string field () const;

        /*!
         * @brief Obtains the current HTTP header's data.
         * @return The current HTTP header's data.
         * @pre @c next() just returned @c true.
         *
         * @see next()
         * @see field()
         */
        std::string value () const;
    };

}

#endif /* _chttp_head_hpp__ */
