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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/timeval.h>
#include <libexplain/settimeofday.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_settimeofday <tv> <tz>\n");
    fprintf(stderr, "       test_settimeofday -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    struct timeval *tv = NULL;
    struct timeval tv_data;
    struct timezone *tz = NULL;

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
    default:
        usage();

    case 2:
         tz = explain_parse_pointer_or_die(argv[optind + 1]);
         /* Fall through... */

    case 1:
        explain_parse_timeval_or_die(argv[optind], "arg one", &tv_data);
        tv = &tv_data;
        break;
    }

    explain_settimeofday_or_die(tv, tz);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
