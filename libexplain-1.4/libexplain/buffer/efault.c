/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009, 2013 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/gettext.h>
#include <libexplain/buffer/software_error.h>
#include <libexplain/string_buffer.h>


void
explain_buffer_efault(explain_string_buffer_t *sb, const char *caption)
{
    explain_string_buffer_printf_gettext
    (
        sb,
        /*
         * xgettext: This message is used when a system call argument
         * points to non-existent memory.  This is usually caused by
         * either a NULL pointer, or an uninitialized variable, or a
         * memory scribble.
         *
         * %1$s => the name of the offending system call argument.
         */
        i18n("%s refers to memory that is outside the process's accessible "
            "address space"),
        caption
    );
    explain_buffer_software_error(sb);
}


/* vim: set ts=8 sw=4 et : */
