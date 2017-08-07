/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CODEGEN_ELASTIC_BUFFER_H
#define CODEGEN_ELASTIC_BUFFER_H

#include <libexplain/ac/stddef.h>
#include <libexplain/ac/stdio.h>

#include <libexplain/gcc_attributes.h>

typedef struct elastic_buffer_t elastic_buffer_t;
struct elastic_buffer_t
{
    char *data;
    size_t data_length;
    size_t data_maximum;
};

/**
  * The elastic_buffer_constructor function may be used to initialize a
  * new instance of a string buffer.
  *
  * @param sb
  *    The string buffer to be inituilized.
  */
void elastic_buffer_constructor(elastic_buffer_t *sb);

/**
  * The elastic_buffer_destructor function may be used to release
  * resources held by an string buffer instance.
  *
  * @param sb
  *    The string buffer to be inituilized.
  */
void elastic_buffer_destructor(elastic_buffer_t *sb);

/**
  * The elastic_buffer_get function may be used to obtain a pointer to
  * the C string being constructed in the string buffer.
  *
  * @param sb
  *     The string buffer of interest.
  * @returns
  *     pointer to a NUL terminated string.  Do not edit, do not
  *     free, do not assume it is still valid after <b>and</b> later
  *     elastic_buffer_* function call.
  */
const char *elastic_buffer_get(elastic_buffer_t *sb);

/**
  * The elastic_buffer_putc function may be used to append a single
  * character to a string buffer.
  *
  * @param sb
  *     The string buffer of interest.
  * @param c
  *     The character to be appended.
  */
void elastic_buffer_putc(elastic_buffer_t *sb, char c);

/**
  * The elastic_buffer_puts function may be used to append a C string to
  * the end of a string buffer.
  *
  * @param sb
  *     The string buffer of interest.
  * @param s
  *     The NUL terminated string to be appended.
  */
void elastic_buffer_puts(elastic_buffer_t *sb, const char *s);

/**
  * The elastic_buffer_printf function may be used to append a formatted
  * string to the end of a string buffer.
  *
  * @param sb
  *     The string buffer of interest.
  * @param fmt
  *     The format specified
  */
void elastic_buffer_printf(elastic_buffer_t *sb, const char *fmt, ...)
                                                 LIBEXPLAIN_FORMAT_PRINTF(2, 3);

/**
  * The elastic_buffer_rewind function may be used to empty a string
  * buffer, in preparation for re-use.
  *
  * @param sb
  *     The string buffer of interest.
  */
void elastic_buffer_rewind(elastic_buffer_t *sb);

/**
  * The elastic_buffer_fwrite function is used to write
  * the contents of a string buffer to the given file.
  *
  * @param sb
  *     The string buffer to be written.
  * @param fp
  *     The file to be written to.
  */
void elastic_buffer_fwrite(elastic_buffer_t *sb, FILE *fp);

#endif /* CODEGEN_ELASTIC_BUFFER_H */
