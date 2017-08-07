/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2013 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/strcoll.h>
#include <libexplain/output.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/strcoll.h>


void
explain_syscall_strcoll(int errnum, int argc, char **argv)
{
    const char      *s1;
    const char      *s2;

    if (argc != 2)
    {
        explain_output_error_and_die
        (
            "strcoll: requires 2 arguments, not %d\n",
            argc
        );
    }
    s1 = argv[0];
    s2 = argv[1];

    explain_wrap_and_print(stdout, explain_errno_strcoll(errnum, s1, s2));
}


/* vim: set ts=8 sw=4 et : */
