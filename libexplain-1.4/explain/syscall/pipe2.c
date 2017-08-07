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

#include <libexplain/buffer/open_flags.h>
#include <libexplain/pipe2.h>
#include <libexplain/output.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/pipe2.h>


void
explain_syscall_pipe2(int errnum, int argc, char **argv)
{
    int fildes_array[2];
    int *fildes = fildes_array;
    int flags = 0;

    switch (argc)
    {
    case 2:
        fildes = explain_parse_pointer_or_die(argv[0]);
        flags = explain_parse_open_flags_or_die(argv[1], "arg 2");
        break;

    case 1:
        flags = explain_parse_open_flags_or_die(argv[0], "arg 1");
        break;

    default:
        explain_output_error_and_die
        (
            "pipe2: requires 2 arguments, not %d\n",
            argc
        );
    }

    explain_wrap_and_print(stdout, explain_errno_pipe2(errnum, fildes, flags));
}


/* vim: set ts=8 sw=4 et : */
