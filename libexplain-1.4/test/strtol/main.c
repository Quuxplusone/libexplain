/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2012 Peter Miller
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

#include <libexplain/strtol.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_strtol <nptr> <endptr> <base>\n");
    fprintf(stderr, "       test_strtol -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    const char      *nptr;
    char            *endptr;
    int             base;
    long            n;

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
    nptr = 0;
    base = 0;
    switch (argc - optind)
    {
    case 2:
        base = explain_strtol_or_die(argv[optind + 1], 0, 0);
        /* fall through... */

    case 1:
        nptr = argv[optind];
        break;

    default:
        usage();
    }
    endptr = 0;
    n = explain_strtol_or_die(nptr, &endptr, base);
    printf("%ld\n", n);
    if (*endptr)
        printf("more text follows the number\n");
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
