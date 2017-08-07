/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2012 Peter Miller
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

#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/sys/stat.h> /* for UTIME_OMIT */
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/timespec.h>
#include <libexplain/buffer/utimensat_fildes.h>
#include <libexplain/buffer/utimensat_flags.h>
#include <libexplain/utimensat.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_utimensat <fildes> <pathname> "
        "[ <time1> [ <time2> [ <flags> ]]]\n");
    fprintf(stderr, "       test_utimensat -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    int             fildes;
    const char      *pathname;
    struct timespec data[2];
    int             flags;

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
    pathname = 0;
    data[0].tv_sec = 0;
    data[0].tv_nsec = UTIME_OMIT;
    data[1].tv_sec = 0;
    data[1].tv_nsec = UTIME_OMIT;
    flags = 0;
    switch (argc - optind)
    {
    case 5:
        flags = explain_parse_utimensat_flags_or_die(argv[optind + 4], "arg5");
        /* Fall through... */

    case 4:
        explain_parse_timespec_or_die(argv[optind + 3], "arg4", &data[1]);
        /* Fall through... */

    case 3:
        explain_parse_timespec_or_die(argv[optind + 2], "arg3", &data[0]);
        /* Fall through... */

    case 2:
        pathname = argv[optind + 1];

        /*
         * Try to be smarter about the directory file description
         * that pathname is relative to.
         */
        fildes = open(argv[optind], O_RDONLY | O_DIRECTORY, 0);
        if (fildes >= 0)
            break;
        fildes = explain_parse_utimensat_fildes_or_die(argv[optind], "arg1");
        break;

    default:
        usage();
        /*NOTREACHED*/
    }
    explain_utimensat_or_die(fildes, pathname, data, flags);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
