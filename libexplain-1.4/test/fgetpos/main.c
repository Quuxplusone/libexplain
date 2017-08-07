/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010, 2012, 2013 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but
 * WITHOUT ANY WARRANTY; without even the implied warranty
 * ofMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNULesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/fpos_t.h>
#include <libexplain/fgetpos.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/version_print.h>
#include <libexplain/wrap_and_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_fgetpos <fp> <pos>\n");
    fprintf(stderr, "       test_fgetpos -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    FILE            *fp;
    fpos_t          apos;
    fpos_t          *pos;

    pos = &apos;
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
        pos = explain_parse_pointer_or_die(argv[optind + 1]);
        /* fall through... */

    case 1:
        fp = explain_parse_pointer_or_die(argv[optind]);
        break;
    }

    explain_fgetpos_or_die(fp, pos);

    if (pos == &apos)
    {
        char text[1000];
        explain_string_buffer_t text_sb;
        explain_string_buffer_init(&text_sb, text, sizeof(text));
        explain_buffer_fpos_t(&text_sb, pos, 1);
        explain_wrap_and_print(stdout, text);
    }

    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
