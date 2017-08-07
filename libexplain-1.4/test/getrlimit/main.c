/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009, 2012 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/sys/resource.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/resource.h>
#include <libexplain/buffer/rlimit.h>
#include <libexplain/getrlimit.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_getrlimit <resource> <rlim>\n");
    fprintf(stderr, "       test_getrlimit -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    int             resource;
    struct rlimit   rlim;
    explain_string_buffer_t buf_sb;
    char            buf[200];

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
    resource = explain_parse_resource_or_die(argv[optind], "resource name");

    explain_getrlimit_or_die(resource, &rlim);
    explain_string_buffer_init(&buf_sb, buf, sizeof(buf));
    explain_buffer_resource(&buf_sb, resource);
    explain_string_buffer_puts(&buf_sb, " = ");
    explain_buffer_rlimit(&buf_sb, &rlim);
    printf("%s\n", buf);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
