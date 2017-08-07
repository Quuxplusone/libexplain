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
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_acl_from_text <text>\n");
    fprintf(stderr, "       test_acl_from_text -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    const char *text = NULL;
    acl_t acl = NULL;

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
    text = argv[optind];
    acl = explain_acl_from_text_or_die(text);
    printf("%s\n", acl_to_text(acl, NULL));
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
