/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2013 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but
 * WITHOUT ANY WARRANTY; without even the implied warranty
 * ofMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNULesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>
#include <libexplain/ac/stdarg.h>

#include <libexplain/buffer/errno/execlp.h>
#include <libexplain/execlp.h>
#include <libexplain/string_buffer.h>


void
explain_message_execlp(char *message, int message_size, const char *pathname,
    const char *arg, ...)
{
    int             errnum;
    va_list         ap;
    explain_string_buffer_t sb;

    errnum = errno;
    va_start(ap, arg);
    explain_string_buffer_init(&sb, message, message_size);
    explain_buffer_errno_execlp(&sb, errnum, pathname, arg, ap);
}


/* vim: set ts=8 sw=4 et : */
