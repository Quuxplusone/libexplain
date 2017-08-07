/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/time.h>
#include <libexplain/ac/utime.h>

#include <libexplain/futimes.h>
#include <libexplain/strtod.h>
#include <libexplain/strtol.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/futimes.h>


static void
timeval_or_die(const char *s, struct timeval *tv)
{
    double          n;

    n = explain_strtod_or_die(s, 0);
    tv->tv_sec = (long)n;
    tv->tv_usec = (long)((n - tv->tv_sec) * 1e6);
}


void
explain_syscall_futimes(int errnum, int argc, char **argv)
{
    int             fildes;
    struct timeval  tv[2];

    switch (argc)
    {
    case 1:
        fildes = explain_strtol_or_die(argv[0], 0, 0);

        explain_wrap_and_print
        (
            stdout,
            explain_errno_futimes(errnum, fildes, 0)
        );
        break;

    case 3:
        fildes = explain_strtol_or_die(argv[0], 0, 0);
        timeval_or_die(argv[1], &tv[0]);
        timeval_or_die(argv[2], &tv[1]);

        explain_wrap_and_print
        (
            stdout,
            explain_errno_futimes(errnum, fildes, tv)
        );
        break;

    default:
        fprintf(stderr, "futimes: requires 3 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
}

/* vim:ts=8:sw=4:et */
