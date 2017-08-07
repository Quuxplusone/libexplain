/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2013 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/output.h>
#include <libexplain/strtoll.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/strtoll.h>


void
explain_syscall_strtoll(int errnum, int argc, char **argv)
{
    const char      *nptr;
    char            *ep;
    char            **endptr;
    int             base;

    nptr = 0;
    ep = 0;
    endptr = &ep;
    base = 0;
    switch (argc)
    {
    case 1:
        nptr = argv[0];
        break;

    case 2:
        nptr = argv[0];
        base = explain_parse_int_or_die(argv[1]);
        break;

    case 3:
        nptr = argv[0];
        endptr = explain_parse_pointer_or_die(argv[1]);
        base = explain_parse_int_or_die(argv[2]);
        break;

    default:
        explain_output_error_and_die
        (
            "strtoll: requires 3 arguments, not %d\n",
            argc
        );
    }

    explain_wrap_and_print(stdout, explain_errno_strtoll(errnum, nptr, endptr,
        base));
}


/* vim: set ts=8 sw=4 et : */
