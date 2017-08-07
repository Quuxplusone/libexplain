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

#include <libexplain/getgrent.h>
#include <libexplain/output.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/getgrent.h>


void
explain_syscall_getgrent(int errnum, int argc, char **argv)
{
    (void)argv;
    if (argc != 0)
    {
        explain_output_error_and_die
        (
            "getgrent: requires 0 arguments, not %d\n",
            argc
        );
    }

    explain_wrap_and_print(stdout, explain_errno_getgrent(errnum));
}


/* vim: set ts=8 sw=4 et : */
