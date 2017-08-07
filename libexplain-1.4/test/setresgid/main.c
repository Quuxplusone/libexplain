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

#include <libexplain/setresgid.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_setresgid <rgid> <egid> <sgid>\n");
    fprintf(stderr, "       test_setresgid -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    gid_t           rgid;
    gid_t           egid;
    gid_t           sgid;

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

    rgid = -1;
    egid = -1;
    sgid = -1;
    switch (argc - optind)
    {
    case 3:
        sgid = explain_parse_gid_t_or_die(argv[optind + 2]);
    case 2:
        egid = explain_parse_gid_t_or_die(argv[optind + 1]);
    case 1:
        rgid = explain_parse_gid_t_or_die(argv[optind]);
        break;

    default:
        usage();
    }

    explain_setresgid_or_die(rgid, egid, sgid);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
