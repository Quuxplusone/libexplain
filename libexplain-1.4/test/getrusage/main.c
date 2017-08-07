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
#include <libexplain/ac/sys/resource.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/getrusage_who.h>
#include <libexplain/buffer/rusage.h>
#include <libexplain/getrusage.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/version_print.h>
#include <libexplain/wrap_and_print.h>


static void
cmd_line_usage(void)
{
    fprintf(stderr, "Usage: test_getrusage <who> <usage>\n");
    fprintf(stderr, "       test_getrusage -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    int             who;
    struct rusage   *usage;
    struct rusage   usage_actual;

    usage = &usage_actual;
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
            cmd_line_usage();
        }
    }
    switch (argc - optind)
    {
    default:
        cmd_line_usage();

    case 2:
        usage = explain_parse_pointer_or_die(argv[optind + 1]);
        /* Fall through... */

    case 1:
        who = explain_parse_getrusage_who_or_die(argv[optind], "argument 1");
        break;
    }

    explain_getrusage_or_die(who, usage);

    {
        char            txt[2000];
        explain_string_buffer_t sb;

        explain_string_buffer_init(&sb, txt, sizeof(txt));
        explain_buffer_rusage(&sb, usage);
        explain_wrap_and_print(stdout, txt);
    }

    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
