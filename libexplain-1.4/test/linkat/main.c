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

#include <libexplain/buffer/fildes.h>
#include <libexplain/buffer/utimensat_flags.h>
#include <libexplain/linkat.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_linkat <old_fildes> <old_path> <new_fildes> "
        "<new_path> [ <flags> ]\n");
    fprintf(stderr, "       test_linkat -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    int             old_fildes = -1;
    const char      *old_path = NULL;
    int             new_fildes = -1;
    const char      *new_path = NULL;
    int             flags = 0;

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

    case 5:
        flags = explain_parse_utimensat_flags_or_die(argv[4], "arg five");
        /* Fall through... */
    case 4:
        new_path = argv[optind + 3];
        new_fildes = explain_parse_fildes_or_die(argv[optind + 2], "arg three");
        old_path = argv[optind + 1];
        old_fildes = explain_parse_fildes_or_die(argv[optind], "arg one");
        break;
    }

    explain_linkat_or_die(old_fildes, old_path, new_fildes, new_path, flags);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
