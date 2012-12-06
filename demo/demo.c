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
 * @brief C demo program.
 */

#include "chttp.h"
#include <string.h>
#include <stdio.h>

int main (int argc, char ** argv)
{
    http_cursor cursor;

    // Prepare.
    http_head head;
    http_head_init(&head, 4*1024);

    // Acquire.
    http_head_push(&head, "Content-Length", "123");
    http_head_push(&head, "Content-Type", "application/json");

    // Iterate.
    http_cursor_init(&cursor, &head);
    while (http_cursor_next(&cursor)) {
        fprintf(stdout, "'%s': '%s'.\n", cursor.field, cursor.value);
    }

    // Search.
    fprintf(stdout, "size: '%s'.\n", http_head_find(&head, "Content-Length"));
    fprintf(stdout, "type: '%s'.\n", http_head_find(&head, "Content-Type"));
    fprintf(stdout, "auth: '%s'.\n", http_head_find(&head, "Authorization"));

    // Release.
    http_head_kill(&head);
}
