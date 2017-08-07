/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2010 Peter Miller
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
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/select.h>

#include <libexplain/select.h>
#include <libexplain/strtol.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/select.h>


void
explain_syscall_select(int errnum, int argc, char **argv)
{
    int             nfds;
    fd_set          readfds;
    fd_set          *readfds_p;
    fd_set          writefds;
    fd_set          *writefds_p;
    fd_set          exceptfds;
    fd_set          *exceptfds_p;
    struct timeval  timeout;

    FD_ZERO(&readfds);
    readfds_p = &readfds;
    FD_ZERO(&writefds);
    writefds_p = &writefds;
    FD_ZERO(&exceptfds);
    exceptfds_p = &exceptfds;
    FD_SET(fileno(stdin), &readfds);
    FD_SET(fileno(stdout), &writefds);
    timeout.tv_sec = 60;
    timeout.tv_usec = 0;
    switch (argc)
    {
    case 5:
        {
            double          seconds;

            seconds = atof(argv[4]);
            timeout.tv_sec = (long)seconds;
            timeout.tv_usec = (long)(1e6 * (seconds - timeout.tv_sec));
        }
        /* fall through... */

    case 4:
        exceptfds_p = (fd_set *)explain_strtol_or_die(argv[3], 0, 0);
        /* fall through... */

    case 3:
        writefds_p = (fd_set *)explain_strtol_or_die(argv[2], 0, 0);
        /* fall through... */

    case 2:
        readfds_p = (fd_set *)explain_strtol_or_die(argv[1], 0, 0);
        /* fall through... */

    case 1:
        nfds = explain_strtol_or_die(argv[0], 0, 0);
        break;

    default:
        fprintf(stderr, "select: requires 5 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print
    (
        stdout,
        explain_errno_select
        (
            errnum,
            nfds,
            readfds_p,
            writefds_p,
            exceptfds_p,
            &timeout
        )
    );
}
