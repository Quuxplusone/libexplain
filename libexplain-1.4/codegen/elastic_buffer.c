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

#include <libexplain/ac/stdarg.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>

#include <libexplain/fwrite.h>
#include <libexplain/malloc.h>

#include <codegen/elastic_buffer.h>


void
elastic_buffer_constructor(elastic_buffer_t *sb)
{
    sb->data = 0;
    sb->data_length = 0;
    sb->data_maximum = 0;
}


void
elastic_buffer_destructor(elastic_buffer_t *sb)
{
    if (sb->data)
        free(sb->data);
    sb->data = 0;
    sb->data_length = 0;
    sb->data_maximum = 0;
}


const char *
elastic_buffer_get(elastic_buffer_t *sb)
{
    return (sb->data_length ? sb->data : "");
}


void
elastic_buffer_putc(elastic_buffer_t *sb, char c)
{
    if (sb->data_length + 2 > sb->data_maximum)
    {
        size_t          new_maximum;
        char            *new_data;

        new_maximum = sb->data_maximum * 2 + 64;
        while (sb->data_length + 2 > new_maximum)
            new_maximum = new_maximum * 2 + 64;
        new_data = explain_malloc_or_die(new_maximum);
        if (sb->data)
        {
            memcpy(new_data, sb->data, sb->data_length);
            free(sb->data);
        }
        sb->data = new_data;
        sb->data_maximum = new_maximum;
    }
    sb->data[sb->data_length++] = c;
    sb->data[sb->data_length] = '\0';
}


void
elastic_buffer_puts(elastic_buffer_t *sb, const char *s)
{
    size_t          len;

    len = strlen(s);
    if (sb->data_length + len + 1 > sb->data_maximum)
    {
        size_t          new_maximum;
        char            *new_data;

        new_maximum = sb->data_maximum * 2 + 64;
        while (sb->data_length + len + 1 > new_maximum)
            new_maximum = new_maximum * 2 + 64;
        new_data = explain_malloc_or_die(new_maximum);
        if (sb->data)
        {
            memcpy(new_data, sb->data, sb->data_length);
            free(sb->data);
        }
        sb->data = new_data;
        sb->data_maximum = new_maximum;
    }
    memcpy(sb->data + sb->data_length, s, len + 1);
    sb->data_length += len;
}


void
elastic_buffer_printf(elastic_buffer_t *sb, const char *fmt, ...)
{
    va_list         ap;
    char            buffer[2000];

    va_start(ap, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    va_end(ap);

    elastic_buffer_puts(sb, buffer);
}


void
elastic_buffer_rewind(elastic_buffer_t *sb)
{
    sb->data_length = 0;
    if (sb->data)
        sb->data[0] = '\0';
}


void
elastic_buffer_fwrite(elastic_buffer_t *sb, FILE *fp)
{
    explain_fwrite_or_die(sb->data, sb->data_length, 1, fp);
}
