/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2012 Peter Miller
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

#include <libexplain/ac/limits.h> /* for PATH_MAX on Solaris */
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/sys/param.h> /* for PATH_MAX except Solaris */
#include <libexplain/ac/unistd.h>

#include <libexplain/readlink.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_readlink <pathname>\n");
    fprintf(stderr, "       test_readlink -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    const char      *pathname;
    int             n;
    char            data[PATH_MAX + 1];

    for (;;)
    {
        int c = getopt(argc, argv, "V");
        if (c == EOF)
            break;
        switch (c)
        {
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

    n = explain_readlink_or_die(pathname, data, sizeof(data));
    printf("%.*s\n", n, data);
    return 0;
}


/* vim: set ts=8 sw=4 et : */
