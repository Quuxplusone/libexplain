/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009, 2012 Peter Miller
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
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/address_family.h>
#include <libexplain/buffer/socket_protocol.h>
#include <libexplain/buffer/socket_type.h>
#include <libexplain/socket.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_socket <domain> <type> <protocol>\n");
    fprintf(stderr, "       test_socket -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    int             domain;
    int             type;
    int             protocol;

    for (;;)
    {
        int c = getopt(argc, argv, "V");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'V':
            explain_version_print();
            return EXIT_SUCCESS;

        default:
            usage();
        }
    }
    if (optind + 3 != argc)
        usage();

    domain = explain_parse_address_family_or_die(argv[optind], "arg 1");
    type = explain_parse_socket_type_or_die(argv[optind + 1], "arg 2");
    protocol =
        explain_parse_socket_protocol_or_die(argv[optind + 2], "arg 3");

    explain_socket_or_die(domain, type, protocol);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
