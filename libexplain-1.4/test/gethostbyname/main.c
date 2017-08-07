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

#include <libexplain/ac/assert.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/hostent.h>
#include <libexplain/gethostbyname.h>
#include <libexplain/version_print.h>
#include <libexplain/wrap_and_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_gethostbyname <name>\n");
    fprintf(stderr, "       test_gethostbyname -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    const char      *name;
    struct hostent  *he;

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
    if (optind + 1 != argc)
        usage();
    name = argv[optind];

    he = explain_gethostbyname_or_die(name);
    assert(he);

    {
        explain_string_buffer_t sb;
        char            buffer[1000];

        explain_string_buffer_init(&sb, buffer, sizeof(buffer));
        explain_buffer_hostent(&sb, he);
        explain_wrap_and_print(stdout, buffer);
    }

    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
