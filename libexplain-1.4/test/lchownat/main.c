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
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/fildes.h>
#include <libexplain/lchownat.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_lchownat <fildes> <pathname> <uid> <gid>\n");
    fprintf(stderr, "       test_lchownat -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    int             fildes = -1;
    const char      *pathname = NULL;
    int             uid = -1;
    int             gid = -1;

    for (;;)
    {
        int c = getopt(argc, argv, "V");
        if (c == -1)
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
        /*NOTREACHED*/
    case 4:
        gid = explain_parse_gid_t_or_die(argv[optind + 3]);
        /* Fall through... */

    case 3:
        uid = explain_parse_uid_t_or_die(argv[optind + 2]);
        /* Fall through... */

    case 2:
        pathname = argv[optind + 1];
        /* Fall through... */

    case 1:
        fildes = explain_parse_fildes_or_die(argv[optind], "arg one");
        break;
    }


    explain_lchownat_or_die(fildes, pathname, uid, gid);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
