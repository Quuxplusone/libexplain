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

#include <libexplain/buffer/address_family.h>
#include <libexplain/buffer/socket_protocol.h>
#include <libexplain/buffer/socket_type.h>
#include <libexplain/socket.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/socket.h>


void
explain_syscall_socket(int errnum, int argc, char **argv)
{
    int             domain;
    int             type;
    int             protocol;

    if (argc != 3)
    {
        fprintf(stderr, "socket: requires 3 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    domain = explain_parse_address_family_or_die(argv[0], "socket arg 1");
    type = explain_parse_socket_type_or_die(argv[1], "socket arg 2");
    protocol = explain_parse_socket_protocol_or_die(argv[2], "socket arg 3");

    explain_wrap_and_print
    (
        stdout,
        explain_errno_socket(errnum, domain, type, protocol)
    );
}
