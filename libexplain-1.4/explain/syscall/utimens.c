/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2012 Peter Miller
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
#include <libexplain/ac/sys/stat.h> /* for UTIME_NOW */

#include <libexplain/buffer/timespec.h>
#include <libexplain/output.h>
#include <libexplain/utimens.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/utimens.h>


void
explain_syscall_utimens(int errnum, int argc, char **argv)
{
    const char      *pathname;
    struct timespec data[2];

    pathname  = 0;
    data[0].tv_sec = 0;
    data[0].tv_nsec = UTIME_OMIT;
    data[1].tv_sec = 0;
    data[1].tv_nsec = UTIME_OMIT;
    switch (argc)
    {
    case 3:
        explain_parse_timespec_or_die(argv[2], "arg3", &data[1]);
        /* Fall through... */

    case 2:
        explain_parse_timespec_or_die(argv[1], "arg2", &data[0]);
        /* Fall through... */

    case 1:
        pathname = argv[0];
        break;

    default:
        explain_output_error_and_die
        (
            "utimens: requires 3 arguments, not %d\n",
            argc
        );
        /*NOTREACHED*/
    }

    explain_wrap_and_print(stdout, explain_errno_utimens(errnum, pathname,
        data));
}


/* vim: set ts=8 sw=4 et : */
