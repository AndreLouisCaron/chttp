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

#include "chttp.h"
#include <malloc.h>
#include <string.h>

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
    const size_t mark = http_head_mark(self);
    if (!http_head_push_field(self, field, strlen(field))) {
        http_head_cancel(self, mark);
        return 0;
    }
    if (!http_head_push_value(self, value, strlen(value))) {
        http_head_cancel(self, mark);
        return 0;
    }
    if (!http_head_commit(self, mark)) {
        http_head_cancel(self, mark);
        return 0;
    }
    return 1;
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

size_t http_head_mark (const http_head * self)
{
    return (self->used);
}

int http_head_push_field (http_head * self, const char * field, size_t size)
{
    size_t used = 0;
    // Check that enough space is remaining.
    if ((self->size-self->used-3) < size) {
        return 0;
    }
    // Copy data.
    while ((used < size) && (field[used] != '\0')) {
        self->data[self->used++] = field[used++];
    }
    // Add null terminator.
    self->data[self->used++] = '\0';
    return 1;
}

int http_head_push_value (http_head * self, const char * value, size_t size)
{
    size_t used = 0;
    // Check that enough space is remaining.
    if ((self->size-self->used-2) < size) {
        return 0;
    }
    // Copy data.
    while ((used < size) && (value[used] != '\0')) {
        self->data[self->used++] = value[used++];
    }
    // Add null terminator.
    self->data[self->used++] = '\0';
    return 1;
}

int http_head_commit (http_head * self, size_t mark)
{
    size_t nulls = 0;
    // Validate the mark.
    if (mark >= (self->size-3)) {
        return 0;
    }
    // Verify that the partial operations put valid null terminators.
    while (mark < self->used) {
        nulls += (self->data[mark++] == '\0');
    }
    if ((nulls != 2) || (self->data[self->used-1] != '\0')) {
        return 0;
    }
    // Restore buffer invariant.
    self->data[self->used] = '\0';
    return 1;
}

int http_head_cancel (http_head * self, size_t mark)
{
    // Validate the mark.
    if ((mark >= (self->size-3))) {
        return 0;
    }
    // Restore buffer invariants.
    self->data[self->used=mark] = '\0';
    return 1;
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
