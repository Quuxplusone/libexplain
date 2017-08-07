/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2010, 2012 Peter Miller
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

#include <libexplain/buffer/pathconf_name.h>
#include <libexplain/fpathconf.h>
#include <libexplain/strtol.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_fpathconf <fildes> <name>\n");
    fprintf(stderr, "       test_fpathconf -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    int             fildes;
    int             name;
    long            result;

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
    if (optind + 2 != argc)
        usage();
    fildes = explain_strtol_or_die(argv[optind], 0, 0);
    name = explain_parse_pathconf_name_or_die(argv[optind + 1]);
    result = explain_fpathconf_or_die(fildes, name);
    printf("%ld\n", result);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
