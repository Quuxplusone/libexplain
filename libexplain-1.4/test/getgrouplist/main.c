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

#include <libexplain/ac/grp.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/sys/types.h>
#include <libexplain/ac/pwd.h>

#include <libexplain/getgrouplist.h>
#include <libexplain/malloc.h>
#include <libexplain/output.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_getgrouplist <user> <ngroups>\n");
    fprintf(stderr, "       test_getgrouplist -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    const char      *user;
    int             ngroups;
    struct passwd   *pw;
    size_t          groups_size;
    gid_t           *groups;
    int             j;

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
    user = NULL;
    ngroups = 0;
    switch (argc - optind)
    {
    case 2:
        ngroups = explain_parse_int_or_die(argv[optind + 1]);
        /* Fall through... */

    case 1:
        user = argv[optind];
        break;

    default:
        usage();
    }
    pw = getpwnam(user);
    if (!pw)
        explain_output_error_and_die("user \"%s\" unknown", user);
    groups_size = (ngroups < 1 ? 1 : ngroups) * sizeof(groups[0]);
    groups = explain_malloc_or_die(groups_size);
    explain_getgrouplist_or_die(user, pw->pw_gid, groups, &ngroups);
    for (j = 0; j < ngroups; ++j)
    {
        gid_t           gid;
        struct group    *gr;

        gid = groups[j];
        printf("%d", gid);
        gr = getgrgid(gid);
        if (gr)
            printf(" \"%s\"", gr->gr_name);
        printf("\n");
    }
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
