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

#include <libexplain/ac/netdb.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/address_family.h>
#include <libexplain/buffer/addrinfo.h>
#include <libexplain/buffer/addrinfo_flags.h>
#include <libexplain/buffer/socket_protocol.h>
#include <libexplain/buffer/socket_type.h>
#include <libexplain/getaddrinfo.h>
#include <libexplain/version_print.h>
#include <libexplain/wrap_and_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_getaddrinfo <node> <service> <hints> <res>\n");
    fprintf(stderr, "       test_getaddrinfo -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    const char      *node;
    const char      *service;
    static const struct addrinfo hints_zero;
    struct addrinfo hints;
    struct addrinfo *res;
    struct addrinfo *p;

    hints = hints_zero;
    node = 0;
    service = 0;
    for (;;)
    {
        int c = getopt(argc, argv, "f:n:o:p:s:t:V");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'f':
            hints.ai_family =
                explain_parse_address_family_or_die(optarg, "-f");
            break;

        case 'n':
            node = optarg;
            break;

        case 'o':
            hints.ai_flags =
                explain_parse_addrinfo_flags_or_die(optarg, "-o");
            break;

        case 'p':
            hints.ai_protocol =
                explain_parse_socket_protocol_or_die(optarg, "-p");
            break;

        case 's':
            service = optarg;
            break;

        case 't':
            hints.ai_socktype =
                explain_parse_socket_type_or_die(optarg, "-t");
            break;

        case 'V':
            explain_version_print();
            return EXIT_SUCCESS;

        default:
            usage();
        }
    }
    if (optind != argc)
        usage();

    explain_getaddrinfo_or_die(node, service, &hints, &res);
    for (p = res; p; p = p->ai_next)
    {
        explain_string_buffer_t buf_sb;
        char            buf[1000];

        explain_string_buffer_init(&buf_sb, buf, sizeof(buf));
        explain_buffer_addrinfo(&buf_sb, p);
        explain_string_buffer_putc(&buf_sb, ',');
        explain_wrap_and_print(stdout, buf);
    }
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
