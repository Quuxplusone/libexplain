/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010 Peter Miller
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
#include <libexplain/ac/sys/socket.h>

#include <libexplain/getsockname.h>
#include <libexplain/strtol.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/getsockname.h>


void
explain_syscall_getsockname(int errnum, int argc, char **argv)
{
    int             fildes;
    struct sockaddr_storage sock_addr;
    struct sockaddr *sock_addr_p;
    socklen_t       sock_addr_size;

    fildes = -1;
    sock_addr_p = (struct sockaddr *)&sock_addr;
    sock_addr_size = sizeof(sock_addr);
    switch (argc)
    {
    case 3:
        sock_addr_size = explain_strtol_or_die(argv[2], 0, 0);
        /* fall through... */

    case 2:
        sock_addr_p = (struct sockaddr *)explain_strtol_or_die(argv[1], 0, 0);
        /* fall through... */

    case 1:
        fildes = explain_strtol_or_die(argv[0], 0, 0);
        break;

    default:
        fprintf(stderr, "getsockname: requires 3 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print
    (
        stdout,
        explain_errno_getsockname
        (
            errnum,
            fildes,
            sock_addr_p,
            &sock_addr_size
        )
    );
}

/* vim:ts=8:sw=4:et */
