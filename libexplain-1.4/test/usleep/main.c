/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2013 Peter Miller
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

#include <libexplain/ac/math.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/string_to_thing.h>
#include <libexplain/strtod.h>
#include <libexplain/usleep.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_usleep <usec>\n");
    fprintf(stderr, "       test_usleep -V\n");
    exit(EXIT_FAILURE);
}


static long long
get_micro_seconds(const char *text)
{
    char *ep = 0;
    double n = explain_strtod_or_die(text, &ep);
    switch (*ep)
    {
    case 's':
        /* seconds */
        break;

    case 'm':
         if (ep[1] == 's')
         {
            /* milli seonds */
            n *= 1e-3;
            break;
         }

         /* minutes */
         n *= 60;
         break;

    case 'h':
         /* hours */
         n *= 60. * 60;
         break;

    case 'd':
         /* days */
         n *= 24. * 60 * 60;
         break;

    default:
    case 'u':
        /* micro seconds */
        n *= 1e-6;
        break;

    case 'n':
        /* nano seconds */
        n *= 1e-9;
        break;
    }
    n *= 1e6;
    return floor(n + 0.5);
}


int
main(int argc, char **argv)
{
    long long       usec;

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
    usec = -1;
    switch (argc - optind)
    {
    default:
        usage();

    case 1:
        usec = get_micro_seconds(argv[optind]);
        break;
    }

    explain_usleep_or_die(usec);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
