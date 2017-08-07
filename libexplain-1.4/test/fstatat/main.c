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

#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/fstatat_flags.h>
#include <libexplain/buffer/stat.h>
#include <libexplain/fstatat.h>
#include <libexplain/open.h>
#include <libexplain/string_buffer.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/version_print.h>
#include <libexplain/wrap_and_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_fstatat <fildes> <pathname> [ <data> ] "
        "<flags>\n");
    fprintf(stderr, "       test_fstatat -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    int             fildes;
    const char      *pathname;
    struct stat     data_var;
    struct stat     *data;
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
    fildes = 0;
    pathname = 0;
    data = &data_var;
    flags = 0;
    switch (argc - optind)
    {
    default:
        usage();

    case 4:
        flags = explain_parse_fstatat_flags_or_die(argv[optind + 3], "arg 4");
        data = explain_parse_pointer_or_die(argv[optind + 2]);
        goto arg2;

    case 3:
        flags = explain_parse_fstatat_flags_or_die(argv[optind + 2], "arg 3");
        /* Fall through... */

    case 2:
        arg2:
        pathname = argv[optind + 1];

    /* case 1: */
        {
            char *ep = 0;
            fildes = strtol(argv[0],  &ep, 0);
            if (argv[0] == ep || *ep)
            {
                fildes = explain_open_or_die(argv[0], O_RDONLY, 0);
            }
        }
        break;
    }

    explain_fstatat_or_die(fildes, pathname, data, flags);

    {
        char text[10000];
        explain_string_buffer_t sb;
        explain_string_buffer_init(&sb, text, sizeof(text));
        explain_buffer_stat(&sb, data);
        explain_wrap_and_print(stdout, text);
    }

    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
