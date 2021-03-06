/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2013 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/sys/stat.h>

#include <libexplain/buffer/eisdir.h>


int
explain_buffer_eisdir(explain_string_buffer_t *sb, const char *pathname,
    const char *caption)
{
    struct stat     st;

    if (stat(pathname, &st) >= 0 && S_ISDIR(st.st_mode))
    {
        explain_string_buffer_printf_gettext
        (
            sb,
            /*
             * xgettext: This message is used to explain an EISDIR
             * reported by an open(2) system call.  You may not open a
             * directory for writing.
             *
             * %1$s => the name of the offending system call argument.
             */
            i18n("%s refers to a directory and the access requested "
                "involved writing"),
            caption
        );
        return 1;
    }
    return 0;
}


/* vim: set ts=8 sw=4 et : */
