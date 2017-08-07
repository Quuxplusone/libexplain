/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2011-2013 Peter Miller
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
#include <libexplain/ac/unistd.h>

#include <libexplain/fputs.h>
#include <libexplain/program_name.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    const char      *prog;

    prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s <string> <fp>\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    const char      *arg1;
    FILE            *arg2;

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

    if (optind + 2 != argc)
        usage();

    arg1 = argv[optind];
    arg2 = explain_parse_pointer_or_die(argv[optind + 1]);

    explain_fputs_or_die(arg1, arg2);

    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
