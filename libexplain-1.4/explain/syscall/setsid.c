/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2011, 2012 Peter Miller
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
#include <libexplain/ac/sys/types.h>

#include <libexplain/setsid.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/setsid.h>


void
explain_syscall_setsid(int errnum, int argc, char **argv)
{
    (void)argv;
    if (argc != 0)
    {
        fprintf(stderr, "setsid: requires 0 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print(stdout, explain_errno_setsid(errnum));
}


/* vim: set ts=8 sw=4 et : */
