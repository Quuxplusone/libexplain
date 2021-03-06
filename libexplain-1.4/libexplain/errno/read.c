/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009, 2013 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/unistd.h>

#include <libexplain/common_message_buffer.h>
#include <libexplain/read.h>


const char *
explain_errno_read(int errnum, int fildes, const void *data,
    size_t data_size)
{
    explain_message_errno_read
    (
        explain_common_message_buffer,
        explain_common_message_buffer_size,
        errnum,
        fildes,
        data,
        data_size
    );
    return explain_common_message_buffer;
}


/* vim: set ts=8 sw=4 et : */
