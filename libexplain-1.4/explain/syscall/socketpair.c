/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010, 2013 Peter Miller
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

#include <libexplain/buffer/address_family.h>
#include <libexplain/buffer/socket_protocol.h>
#include <libexplain/buffer/socket_type.h>
#include <libexplain/socketpair.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/socketpair.h>


void
explain_syscall_socketpair(int errnum, int argc, char **argv)
{
    int             domain;
    int             type;
    int             protocol;
    int             sv[2];
    int             *svp;

    domain = 0;
    type = 0;
    protocol = 0;
    svp = sv;
    switch (argc)
    {
    case 4:
        svp = explain_parse_pointer_or_die(argv[3]);
        /* fall through... */

    case 3:
        protocol = explain_parse_socket_protocol_or_die(argv[2], "arg 3");
        /* fall through... */

    case 2:
        type = explain_parse_socket_type_or_die(argv[1], "arg 2");
        /* fall through... */

    case 1:
        domain = explain_parse_address_family_or_die(argv[0], "arg 1");
        break;

    default:
        fprintf(stderr, "socketpair: requires 4 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print(stdout, explain_errno_socketpair(errnum, domain,
        type, protocol, svp));
}


/* vim: set ts=8 sw=4 et : */
