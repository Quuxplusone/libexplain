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
#include <libexplain/ac/sys/stat.h>

#include <libexplain/buffer/fildes.h>
#include <libexplain/buffer/timeval.h>
#include <libexplain/futimesat.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_futimesat <fildes> <pathname> <data>\n");
    fprintf(stderr, "       test_futimesat -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    int             fildes;
    const char      *pathname;
    struct timeval  data[2];

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
    fildes = -1;
    pathname = NULL;
    data[0].tv_sec = 0;
    data[0].tv_usec = UTIME_OMIT;
    data[1].tv_sec = 0;
    data[1].tv_usec = UTIME_OMIT;
    switch (argc - optind)
    {
    default:
        usage();

    case 4:
        explain_parse_timeval_or_die(argv[optind + 3], "arg four", &data[1]);
        /* Fall through.... */

    case 3:
        explain_parse_timeval_or_die(argv[optind + 2], "arg three", &data[0]);
        /* Fall through.... */

    case 2:
        pathname = argv[optind + 1];
        /* Fall through.... */

    case 1:
        fildes = explain_parse_fildes_or_die(argv[optind], "arg one");
        break;
    }

    explain_futimesat_or_die(fildes, pathname, data);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
