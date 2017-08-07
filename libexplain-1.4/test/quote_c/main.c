/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2012 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/output.h>
#include <libexplain/program_name.h>
#include <libexplain/string_buffer.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    const char      *prog;

    prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    for (;;)
    {
        int             c;

        c = getopt(argc, argv, "V");
        if (c < 0)
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
    if (optind == argc)
        usage();
    for (;;)
    {
        char            buffer1[100];
        char            buffer2[sizeof(buffer1)];
        explain_string_buffer_t sb1;
        explain_string_buffer_t sb2;
        const char      *s;

        s = argv[optind++];
        explain_string_buffer_init(&sb1, buffer1, sizeof(buffer1));
        explain_string_buffer_init(&sb2, buffer2, sizeof(buffer2));
        explain_string_buffer_puts_quoted(&sb1, s);
        explain_string_buffer_puts_quoted_n(&sb2, s, strlen(s));

        if (0 != strcmp(buffer1, buffer2))
        {
            explain_output_error_and_die
            (
                "explain_string_buffer_puts_quoted gives %s, but "
                "explain_string_buffer_puts_quoted_n gives %s, oops",
                buffer1,
                buffer2
            );
        }
        fputs(buffer1, stdout);
        putchar('\n');

        if (optind >= argc)
            break;
    }
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
