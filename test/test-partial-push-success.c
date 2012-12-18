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
 * @brief Test for a successful partial push sequence.
 */

#include <chttp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv)
{
    const char field[] = "Content-Lenght";
    const char value[] = "201";
    const char * match = 0;
    http_mark mark;

    http_head head;
    http_head_init(&head, 4*1024);

    // Start a partial push.
    if (!http_head_mark(&head, &mark))
    {
        fprintf(stderr, "Could not start operation.\n");
        return (EXIT_FAILURE);
    }

    // Check that a single call to each part succeeds.
    if (!http_head_push_field(&mark, field, strlen(field)))
    {
        fprintf(stderr, "Could not push field.\n");
        return (EXIT_FAILURE);
    }
    if (!http_head_push_value(&mark, value, strlen(value)))
    {
        fprintf(stderr, "Could not push value.\n");
        return (EXIT_FAILURE);
    }
    if (!http_head_commit(&mark))
    {
        fprintf(stderr, "Could not commit.\n");
        return (EXIT_FAILURE);
    }

    // Verify that the header value is found and matches.
    match = http_head_find(&head, field);
    if (strlen(match) == 0)
    {
        fprintf(stderr, "Header not found.\n");
        return (EXIT_FAILURE);
    }
    if (strcmp(match, value) != 0)
    {
        fprintf(stderr, "Header value doesn't match.\n");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}
