/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010, 2012, 2013 Peter Miller
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
#include <libexplain/ac/unistd.h>

#include <libexplain/ungetc.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_ungetc <c> <fp>\n");
    fprintf(stderr, "       test_ungetc -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    int             c;
    FILE            *fp;

    for (;;)
    {
        int k = getopt(argc, argv, "V");
        if (k == EOF)
            break;
        switch (k)
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
    c = explain_parse_int_or_die(argv[optind]);
    fp = explain_parse_pointer_or_die(argv[optind + 1]);

    explain_ungetc_or_die(c, fp);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
