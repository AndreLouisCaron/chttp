#ifndef _chttp_h__
#define _chttp_h__

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
 * @brief C API definition.
 */

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Buffer for HTTP headers.
 *
 * The buffer uses a flat representation, all values are stored into a single
 * contiguous chunk of memory.  Operations on the buffer are very limited.
 * Only append, iterate and find operations are supported.
 *
 * This representation is convenient for long-lived processes where memory
 * fragmentation is a problem and where there is little need to edit HTTP
 * headers in-place (removing an HTTP header requires seletively copying all
 * other headers to another @c http_head buffer).
 *
 * @see http_head_init
 * @see http_head_kill
 * @see http_head_push
 * @see http_head_find
 * @see http_cursor
 */
typedef struct http_head
{
    /*!
     * @private
     * @brief Pointer to the memory chunk containing the headers.
     * @invariant Points to at least @c size bytes.
     * @invariant @c data[used] is a null character.
     */
    char * data;

    /*!
     * @private
     * @brief Buffer capacity.
     * @brief Size 
     */
    size_t size;

    /*!
     * @private
     * @brief Current buffer usage.
     * @invariant Less than or equal to @c size.
     */
    size_t used;

} http_head;

/*!
 * @brief Create an empty buffer with a capacity of @a size bytes.
 * @param self
 * @param size Buffer capacity (fixed).
 * @return 0 if memory allocation fails, else non-zero.
 */
int http_head_init (http_head * self, size_t size);

/*!
 * @brief Release the chunk of memory held by the buffer.
 * @param self
 * @pre @c http_head_init has been called.
 * @post @c http_head_init can be called again to recycle the buffer instance.
 */
void http_head_kill (http_head * self);

/*!
 * @brief Append an HTTP header to the buffer.
 * @param self
 * @param field HTTP header name.
 * @param value HTTP header data.
 * @return 0 on failure (e.g. attempted to exceed the buffer capacity), else
 *  non-zero.
 */
int http_head_push (http_head * self, const char * field, const char * value);

/*!
 * @brief Search for an HTTP header by name.
 * @param self
 * @param field The name of the HTTP header to look for.
 * @return @c An empty (zero-length) string if the header was not found, else a
 *  null-terminated string containing the HTTP header data.
 *
 * This method is meant for localized processing of specific HTTP headers (e.g.
 * the content length).  Since it always performs a linear search from the
 * start of the buffer, it is usually more efficient to use an @c http_cursor
 * to iterate over all headers.
 *
 * @see http_cursor
 */
const char * http_head_find (const http_head * self, const char * field);

/*!
 * @brief Take the current position in the stream.
 * @param self
 * @return A value suitable for passing to @c http_head_cancel.
 *
 * @see http_head_cancel
 * @see http_head_push_field
 * @see http_head_push_value
 */
size_t http_head_mark (const http_head * self);

/*!
 * @brief Appends partial header data.
 * @param self
 * @param field Part of the HTTP header name.
 * @param size Number of valid bytes starting at @a field.
 * @return 0 on failure (e.g. attempted to exceed the buffer capacity), else
 *  non-zero.
 *
 * @pre The caller has acquired a mark using @c http_head_mark.
 * @post The buffer's invariants are broken, iteration using @c http_cursor is
 *  prohibited.  A successful call to @c http_head_cancel or @c
 *  http_head_commit is required to restore the buffer invariants.
 *
 * @see http_head_mark
 * @see http_head_push_value
 */
int http_head_push_field (http_head * self, const char * field, size_t size);

/*!
 * @brief Appends partial header data.
 * @param self
 * @param value Part of the HTTP header data.
 * @param size Number of valid bytes starting at @a value.
 * @return 0 on failure (e.g. attempted to exceed the buffer capacity), else
 *  non-zero.
 *
 * @pre @c http_head_push_field just succeeded.
 *
 * @see http_head_mark
 * @see http_head_push_field
 */
int http_head_push_value (http_head * self, const char * value, size_t size);

/*!
 * @brief Restore the header invariants after a successful partial push.
 * @param self
 * @param mark A value obtained using @c http_head_mark before the partial push
 *  operation began.
 * @return 0 on failure (e.g. invariants are not respected), else non-zero.
 *
 * @pre @c http_head_push_value just succeeded.
 * @post The buffer's invariants are restored.  If the function returns 0, and
 *  the sequence of partial push operations is cancelled.  If the function
 *  returns non-zero, @a self does not include the partial push operations.
 *
 * @see http_head_push_field
 * @see http_head_push_value
 * @see http_head_cancel
 */
int http_head_commit (http_head * self, size_t mark);

/*!
 * @brief Restore a previous state after a failed partial push.
 * @param self
 * @param mark A value obtained using @c http_head_mark before the partial push
 *  operation began.
 * @return 0 on failure (e.g. @a mark is invalid), else non-zero.
 *
 * @pre A call to @c http_head_push_value or @c http_head_push_field just
 *  failed.
 * @post If the function succeeds, the buffer's invariants are restored.
 *
 * @see http_head_mark
 * @see http_head_push_field
 * @see http_head_push_value
 * @see http_head_commit
 */
int http_head_cancel (http_head * self, size_t mark);

/*!
 * @brief Iterator for HTTP headers.
 *
 * Recommended use:
 * @code
 *  http_head head;
 *  // ...
 *  // Do something that buffers the HTTP headers.
 *  // ...
 *  // OK, we're ready to process the HTTP request.
 *  http_cursor cursor;
 *  http_cursor_init(&cursor, &head);
 *  while (http_cursor_next(&cursor))
 *  {
 *    // Process the header.
 *    if (strcmp(cursor.field, "Content-Length")==0) {
 *        int content_length = atoi(cursor.value);
 *        // ...
 *    }
 *  }
 * @endcode
 *
 * @see http_head_find
 */
typedef struct http_cursor
{
    /*!
     * @private
     * @brief Reference to the buffer over which we're iterating.
     */
    const http_head * head;

    /*!
     * @private
     * @brief Offset in @c head at which to resume the iteration.
     * @invariant 0 <= @c base <= @c head-> used.
     */
    size_t base;

    /*!
     * @public
     * @brief HTTP header name.
     *
     * This pointer is only valid after a call to @c http_cursor_next with a
     * non-zero return value.
     */
    const char * field;

    /*!
     * @public
     * @brief HTTP header data.
     *
     * This pointer is only valid after a call to @c http_cursor_next with a
     * non-zero return value.
     */
    const char * value;

} http_cursor;

/*!
 * @brief Prepare for iteration over HTTP headers.
 * @param self
 * @param head The HTTP headers over which to iterate.
 * @post @a self is ready for the first call to @c http_cursor_next.  Iteration
 *  has not started.
 *
 * @see http_cursor
 * @see http_cursor_next
 */
void http_cursor_init (http_cursor * self, const http_head * head);

/*!
 * @brief Fetch the next HTTP header.
 * @param self The cursor.
 * @return 0 if no more results were available, else 1.
 * @pre @c http_cursor_init was just called or @c http_cursor_next returned 1.
 * @post @c self->field and @c self->value point to an HTTP header's name and
 *  value, respectively.  If the return value is 0, they both point to empty
 *  (zero-length) strings.
 *
 * @see http_cursor
 * @see http_cursor_init
 */
int http_cursor_next (http_cursor * self);

#ifdef __cplusplus
}
#endif

#endif /* _chttp_h__ */
