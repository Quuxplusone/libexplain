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
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/timeval.h>
#include <libexplain/lutimes.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_lutimes <pathname> <data>\n");
    fprintf(stderr, "       test_lutimes -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    const char      *pathname;
    struct timeval  data[2];

    pathname = 0;
    data[0].tv_sec = 0;
    data[0].tv_usec = UTIME_OMIT;
    data[1].tv_sec = 0;
    data[1].tv_usec = UTIME_OMIT;
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
        explain_parse_timeval_or_die(argv[optind + 2], "arg three", &data[1]);
        /* Fall through... */

    case 2:
        explain_parse_timeval_or_die(argv[optind + 1], "arg two", &data[0]);
        /* Fall through... */

    case 1:
        pathname = argv[optind];
        break;

    default:
        usage();
    }

    explain_lutimes_or_die(pathname, data);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
