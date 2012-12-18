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
 * @brief Test that validates that a overflow detection succeeds.
 */

#include <chttp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv)
{
    const char field[] = "Content-Lenght";
    const char value[] = "4096";
    http_mark mark;

    http_head head;
    http_head_init(&head, 10);

    // Verify our test data :-)
    if (strlen(field)+1 <= head.size)
    {
        fprintf(stderr, "Test buffer too large.\n");
        return (EXIT_FAILURE);
    }

    // Start a partial push.
    if (!http_head_mark(&head, &mark))
    {
        fprintf(stderr, "Could not start operation.\n");
        return (EXIT_FAILURE);
    }

    // Attemp to push more data than the buffer can hold.
    if (http_head_push_field(&mark, field, strlen(field)))
    {
        fprintf(stderr, "Field push should fail.\n");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}
