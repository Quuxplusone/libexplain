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

#include <libexplain/buffer/timespec.h>
#include <libexplain/nanosleep.h>
#include <libexplain/output.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/nanosleep.h>


void
explain_syscall_nanosleep(int errnum, int argc, char **argv)
{
    struct timespec req;
    struct timespec rem;

    switch (argc)
    {
    case 2:
        explain_parse_timespec_or_die(argv[1], "arg two", &rem);
        /* Fall through... */

    case 1:
        explain_parse_timespec_or_die(argv[0], "arg one", &req);
        break;

    default:
        explain_output_error_and_die("nanosleep: requires 2 arguments, "
            "not %d\n", argc);
        break;
    }

    explain_wrap_and_print(stdout, explain_errno_nanosleep(errnum, &req, &rem));
}


/* vim: set ts=8 sw=4 et : */
