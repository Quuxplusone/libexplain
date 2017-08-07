/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009, 2012 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/permission_mode.h>
#include <libexplain/mkdir.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_mkdir <pathname>\n");
    fprintf(stderr, "       test_mkdir -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    const char      *pathname;
    int             mode;

    mode = 0777;
    for (;;)
    {
        int c = getopt(argc, argv, "m:V");
        if (c == EOF)
            break;
        switch (c)
        {
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

    explain_mkdir_or_die(pathname, mode);
    return 0;
}


/* vim: set ts=8 sw=4 et : */
