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
 * @brief C API implementation.
 */

#include "head.h"
#include <malloc.h>
#include <string.h>

static size_t push_segment (char * data, const char * text, size_t size)
{
    size_t used = 0;
    while ((used < size-2) && (*text != '\0')) {
        *data++ = *text++, ++used;
    }
    if (*text != '\0') {
        //  Out of space!
    }
    *data++ = '\0', ++used;
    *data++ = '\0'; // Final null terminator (not counted).
    return (used);
}

static size_t next_segment (char ** segment)
{
    size_t used = 0;
    while (**segment != '\0') {
        ++*segment, ++used;
    }
    ++*segment, ++used;
    return (used);
}

int http_head_init (http_head * self, size_t size)
{
    self->data = malloc(self->size=size), self->used = 0;
    return (self->data != 0);
}

void http_head_kill (http_head * self)
{
    free(self->data), self->data = 0, self->used = self->size = 0;
}

int http_head_push (http_head * self, const char * field, const char * value)
{
    // Push header name.
    self->used += push_segment
        (self->data+self->used, field, self->size-self->used);
    // Push header data.
    self->used += push_segment
        (self->data+self->used, value, self->size-self->used);
    return (0);
}

const char * http_head_find (const http_head * self, const char * field)
{
    http_cursor cursor;
    http_cursor_init(&cursor, self);
    while (http_cursor_next(&cursor))
    {
        if (stricmp(cursor.field, field) == 0) {
            return (cursor.value);
        }
    }
    return ("");
}

void http_cursor_init (http_cursor * self, const http_head * head)
{
    self->head = head;
    self->base = 0;
    self->field = self->value = 0;
}

int http_cursor_next (http_cursor * self)
{
    char * text = self->head->data + self->base;
    // Guard against empty head & extra iterations.
    if (*text == '\0') {
        self->field = self->value = "";
        return (0);
    }
    self->field = text, self->base += next_segment(&text);
    self->value = text, self->base += next_segment(&text);
    return (1);
}
