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

#include <libexplain/acl_from_text.h>
#include <libexplain/acl_to_text.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_acl_to_text <acl> <len_p>\n");
    fprintf(stderr, "       test_acl_to_text -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    acl_t acl = acl_from_text("u::rwx,g::r-x,o::r-x");
    ssize_t len;
    ssize_t *len_p = &len;
    char *text = NULL;

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
        len_p = explain_parse_pointer_or_die(argv[optind + 1]);
        /* Fall through... */

    case 1:
        acl = explain_acl_from_text_or_die(argv[optind]);
        break;
    }

    text = explain_acl_to_text_or_die(acl, len_p);
    printf("%s (%d)\n", text, (int)len);
    acl_free(text);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
