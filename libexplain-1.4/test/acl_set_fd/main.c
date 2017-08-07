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
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/acl.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/acl_from_text.h>
#include <libexplain/acl_set_fd.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_acl_set_fd <fildes> <acl>\n");
    fprintf(stderr, "       test_acl_set_fd -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    int fildes = -1;
    acl_t acl = acl_from_text("u::rw-,g::r--,o::r--");

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

    case 2:
        if (0 == strcmp(argv[optind + 1], "NULL"))
            acl = NULL;
        else
            acl = explain_acl_from_text_or_die(argv[optind + 1]);
        /* Fall through... */

    case 1:
        fildes = explain_parse_int_or_die(argv[optind]);
        break;
    }

    explain_acl_set_fd_or_die(fildes, acl);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
