/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2012, 2013 Peter Miller
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

#include <libexplain/strtoll.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_strtoll <nptr> [[ <endptr> ] <base> ]\n");
    fprintf(stderr, "       test_strtoll -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    const char      *nptr;
    char            *ep;
    char            **endptr;
    int             base;
    long long       result;

    nptr = 0;
    ep = 0;
    endptr = &ep;
    base = 0;
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
    switch (argc - optind)
    {
    case 3:
        nptr = argv[optind];
        endptr = explain_parse_pointer_or_die(argv[optind + 1]);
        base = explain_parse_int_or_die(argv[optind + 2]);
        break;

    case 2:
        nptr = argv[optind];
        base = explain_parse_int_or_die(argv[optind + 1]);
        break;

    case 1:
        nptr = argv[optind];
        break;

    default:
        usage();
    }

    result = explain_strtoll_or_die(nptr, endptr, base);
    printf("%lld\n", result);
    if (ep && *ep)
        printf("more text (\"%s\") follows number\n", ep);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
