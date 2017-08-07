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
#include <libexplain/ac/sys/stat.h>

#include <libexplain/futimesat.h>
#include <libexplain/buffer/fildes.h>
#include <libexplain/buffer/timeval.h>
#include <libexplain/output.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/futimesat.h>


void
explain_syscall_futimesat(int errnum, int argc, char **argv)
{
    int             fildes;
    const char      *pathname;
    struct timeval  data[2];

    fildes = -1;
    pathname = NULL;
    data[0].tv_sec = 0;
    data[0].tv_usec = UTIME_OMIT;
    data[1].tv_sec = 0;
    data[1].tv_usec = UTIME_OMIT;
    switch (argc)
    {
    case 4:
         explain_parse_timeval_or_die(argv[3], "arg four", &data[1]);
         /* Fall through... */

    case 3:
         explain_parse_timeval_or_die(argv[2], "arg three", &data[0]);
         /* Fall through... */

    case 2:
         pathname = argv[1];
         /* Fall through... */

    case 1:
         fildes = explain_parse_fildes_or_die(argv[0], "arg one");
         break;

    default:
        explain_output_error_and_die
        (
            "futimesat: requires 3 o 4 arguments, not %d\n",
            argc
        );
    }

    explain_wrap_and_print
    (
        stdout,
        explain_errno_futimesat(errnum, fildes, pathname, data)
    );
}


/* vim: set ts=8 sw=4 et : */
