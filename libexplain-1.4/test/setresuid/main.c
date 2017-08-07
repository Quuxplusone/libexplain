/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2012, 2013 Peter Miller
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
#include <libexplain/ac/unistd.h>

#include <libexplain/setresuid.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_setresuid <ruid> <euid> <suid>\n");
    fprintf(stderr, "       test_setresuid -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    uid_t           ruid;
    uid_t           euid;
    uid_t           suid;

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
    ruid = -1;
    euid = -1;
    suid = -1;
    switch (argc)
    {
    case 3:
        suid = explain_parse_uid_t_or_die(argv[optind + 2]);
    case 2:
        euid = explain_parse_uid_t_or_die(argv[optind + 1]);
    case 1:
        ruid = explain_parse_uid_t_or_die(argv[optind]);
        break;

    default:
        usage();
    }

    explain_setresuid_or_die(ruid, euid, suid);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
