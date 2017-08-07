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
#include <libexplain/ac/sys/utsname.h>

#include <libexplain/buffer/utsname.h>
#include <libexplain/fflush.h>
#include <libexplain/string_buffer.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/uname.h>
#include <libexplain/version_print.h>
#include <libexplain/wrap_and_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_uname <data>\n");
    fprintf(stderr, "       test_uname -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    struct utsname  data;
    struct utsname  *data_ptr;

    data_ptr = &data;
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

    case 1:
        data_ptr = explain_parse_pointer_or_die(argv[optind]);
        break;

    case 0:
        break;
    }

    explain_uname_or_die(data_ptr);

    {
        explain_string_buffer_t sb;
        char text[1000];
        explain_string_buffer_init(&sb, text, sizeof(text));
        explain_buffer_utsname(&sb, data_ptr);
        explain_wrap_and_print(stdout, text);
        explain_fflush_or_die(stdout);
    }
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
