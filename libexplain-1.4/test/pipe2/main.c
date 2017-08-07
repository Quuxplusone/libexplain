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

#include <libexplain/buffer/open_flags.h>
#include <libexplain/pipe2.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_pipe2 [ <fildes> ] <flags>\n");
    fprintf(stderr, "       test_pipe2 -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    int fildes_array[2];
    int *fildes = fildes_array;
    int flags = 0;

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
    case 1:
        flags = explain_parse_open_flags_or_die(argv[optind], "arg 1");
        break;

    case 2:
        fildes = explain_parse_pointer_or_die(argv[optind]);
        flags = explain_parse_open_flags_or_die(argv[optind + 1], "arg 2");
        break;

    default:
        usage();
    }

    explain_pipe2_or_die(fildes, flags);
    if (fildes == fildes_array)
        printf("%d %d\n", fildes_array[0], fildes_array[1]);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
