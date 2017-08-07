/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010, 2012, 2013 Peter Miller
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
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/lseek_whence.h>
#include <libexplain/fseek.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_fseek <fp> <offset> <whence>\n");
    fprintf(stderr, "       test_fseek -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    FILE            *fp;
    long            offset;
    int             whence;

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
    fp = 0;
    offset = 0;
    whence = SEEK_SET;
    switch (argc - optind)
    {
    default:
        usage();

    case 3:
        whence = explain_lseek_whence_parse_or_die(argv[optind + 2], "arg 3");
        /* fall through... */

    case 2:
        offset = explain_parse_long_or_die(argv[optind + 1]);
        /* fall through... */

    case 1:
        fp = explain_parse_pointer_or_die(argv[optind]);
        break;
    }

    explain_fseek_or_die(fp, offset, whence);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
