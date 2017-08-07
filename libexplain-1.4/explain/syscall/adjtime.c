/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2013 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but
 * WITHOUT ANY WARRANTY; without even the implied warranty
 * ofMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNULesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/adjtime.h>
#include <libexplain/buffer/timeval.h>
#include <libexplain/output.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/adjtime.h>


void
explain_syscall_adjtime(int errnum, int argc, char **argv)
{
    struct timeval delta;
    struct timeval old_delta;
    struct timeval *old_delta_p;

    old_delta_p = &old_delta;
    switch (argc)
    {
    case 2:
        old_delta_p = explain_parse_pointer_or_die(argv[1]);
        /* fall through */

    case 1:
        explain_parse_timeval_or_die(argv[0], "arg one", &delta);
        break;

    default:
        explain_output_error_and_die
        (
            "adjtime: requires 2 arguments, not %d\n",
            argc - optind
        );
    }

    explain_wrap_and_print(stdout, explain_errno_adjtime(errnum, &delta,
        old_delta_p));
}


/* vim: set ts=8 sw=4 et : */
