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
#include <libexplain/ac/sys/stat.h> /* for UTIME_OMIT */

#include <libexplain/buffer/timespec.h>
#include <libexplain/buffer/utimensat_fildes.h>
#include <libexplain/buffer/utimensat_flags.h>
#include <libexplain/output.h>
#include <libexplain/utimensat.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/utimensat.h>


void
explain_syscall_utimensat(int errnum, int argc, char **argv)
{
    int             fildes;
    const char      *pathname;
    struct timespec data[2];
    int             flags;

    fildes = -1;
    pathname = 0;
    data[0].tv_sec = 0;
    data[0].tv_nsec = UTIME_OMIT;
    data[1].tv_sec = 0;
    data[1].tv_nsec = UTIME_OMIT;
    flags = 0;
    switch (argc)
    {
    case 5:
        flags = explain_parse_utimensat_flags_or_die(argv[4], "arg5");
        /* Fall through... */

    case 4:
        explain_parse_timespec_or_die(argv[3], "arg4", &data[1]);
        /* Fall through... */

    case 3:
        explain_parse_timespec_or_die(argv[2], "arg3", &data[0]);
        /* Fall through... */

    case 2:
        pathname = argv[1];

        fildes = explain_parse_utimensat_fildes_or_die(argv[0], "arg1");
        break;

    default:
        explain_output_error_and_die
        (
            "utimensat: requires 5 arguments, not %d\n",
            argc
        );
        /*NOTREACHED*/
    }

    explain_wrap_and_print
    (
        stdout,
        explain_errno_utimensat(errnum, fildes, pathname, data, flags)
    );
}


/* vim: set ts=8 sw=4 et : */
