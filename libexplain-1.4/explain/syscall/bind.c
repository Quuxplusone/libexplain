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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/sys/socket.h>

#include <libexplain/bind.h>
#include <libexplain/strtol.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/bind.h>


void
explain_syscall_bind(int errnum, int argc, char **argv)
{
    int             fildes;
    struct sockaddr_storage sas;
    struct sockaddr *sock_addr;
    long            sock_addr_size;

    fildes = -1;
    sock_addr = (struct sockaddr *)&sas;
    sock_addr_size = sizeof(sas);
    switch (argc)
    {
    case 3:
        sock_addr_size = explain_strtol_or_die(argv[2], 0, 0);
        sock_addr = (struct sockaddr *)explain_strtol_or_die(argv[1], 0, 0);
        /* fall through... */

    case 1:
        fildes = explain_strtol_or_die(argv[0], 0, 0);
        break;

    default:
        fprintf(stderr, "bind: requires 3 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print
    (
        stdout,
        explain_errno_bind(errnum, fildes, sock_addr, sock_addr_size)
    );
}
