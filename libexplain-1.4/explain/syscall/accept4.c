/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010, 2013 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but
 * WITHOUT ANY WARRANTY; without even the implied warranty
 * ofMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNULesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/sys/socket.h>

#include <libexplain/accept4.h>
#include <libexplain/buffer/accept4_flags.h>
#include <libexplain/buffer/fildes.h>
#include <libexplain/output.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/accept4.h>


void
explain_syscall_accept4(int errnum, int argc, char **argv)
{
    int             fildes;
    static struct sockaddr data_zero;
    struct sockaddr data;
    struct sockaddr *sock_addr;
    socklen_t       data_size;
    socklen_t       *sock_addr_size;
    int             flags;

    fildes = 0;
    data = data_zero;
    sock_addr = &data;
    data_size = sizeof(data);
    sock_addr_size = &data_size;
    flags = 0;
    switch (argc)
    {
    case 1:
        fildes = explain_parse_fildes_or_die(argv[0], "arg one");
        break;

    case 2:
        fildes = explain_parse_fildes_or_die(argv[0], "arg one");
        flags = explain_accept4_flags_parse_or_die(argv[1], "arg 2");
        break;

    case 4:
        fildes = explain_parse_fildes_or_die(argv[0], "arg one");
        sock_addr = explain_parse_pointer_or_die(argv[1]);
        sock_addr_size = explain_parse_pointer_or_die(argv[2]);
        flags = explain_accept4_flags_parse_or_die(argv[3], "arg 4");
        break;

    default:
        explain_output_error_and_die
        (
            "accept4: requires 4 arguments, not %d\n",
            argc
        );
    }

    explain_wrap_and_print(stdout, explain_errno_accept4(errnum, fildes,
        sock_addr, sock_addr_size, flags));
}


/* vim: set ts=8 sw=4 et : */
