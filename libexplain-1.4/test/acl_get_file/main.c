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
#include <libexplain/ac/sys/acl.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/acl_get_file.h>
#include <libexplain/buffer/acl_type.h>
#include <libexplain/strtol.h>
#include <libexplain/version_print.h>
#include <libexplain/wrap_and_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_acl_get_file <pathname> <type>\n");
    fprintf(stderr, "       test_acl_get_file -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    const char      *pathname;
    acl_type_t      type;
    acl_t           result;
    char            *text;
    ssize_t          size;

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
    if (optind + 2 != argc)
        usage();
    pathname = argv[optind];
    type = explain_acl_type_parse_or_die(argv[optind + 1], "arg 2");

    result = explain_acl_get_file_or_die(pathname, type);

    text = acl_to_text(result, &size);
    explain_wrap_and_print(stdout, text);
    acl_free(text);

    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
