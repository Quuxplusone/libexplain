/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009, 2012, 2013 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/open_flags.h>
#include <libexplain/buffer/permission_mode.h>
#include <libexplain/close.h>
#include <libexplain/open.h>
#include <libexplain/version_print.h>
#include <libexplain/write.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_open [ <option>... ] <pathname>\n");
    fprintf(stderr, "       test_open -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    int             fildes;
    const char      *pathname;
    int             flags;
    int             mode;

    flags = O_RDONLY;
    mode = 0666;
    for (;;)
    {
        int c = getopt(argc, argv, "f:m:V");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'f':
            flags = explain_parse_open_flags_or_die(optarg, "-f");
            break;

        case 'm':
            mode = explain_permission_mode_parse_or_die(optarg, "-m");
            break;

        case 'V':
            explain_version_print();
            return 0;

        default:
            usage();
        }
    }
    if (optind + 1 != argc)
        usage();
    pathname = argv[optind];

    fildes = explain_open_or_die(pathname, flags, mode);
    if ((flags & O_ACCMODE) != O_RDONLY)
        explain_write_or_die(fildes, "This is a test.\n", 16);
    explain_close_or_die(fildes);
    return 0;
}


/* vim: set ts=8 sw=4 et : */
