/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2012 Peter Miller
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
#include <libexplain/ac/time.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/time_t.h>
#include <libexplain/stime.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_stime <t>\n");
    fprintf(stderr, "       test_stime -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    time_t          t;

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
    if (optind + 1 != argc)
        usage();
    t = explain_parse_time_t_or_die(argv[optind], "arg 1");

    explain_stime_or_die(&t);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
