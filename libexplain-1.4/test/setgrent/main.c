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
#include <libexplain/ac/grp.h>

#include <libexplain/endgrent.h>
#include <libexplain/getgrent.h>
#include <libexplain/setgrent.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_setgrent\n");
    fprintf(stderr, "       test_setgrent -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
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
    if (optind + 0 != argc)
        usage();

    explain_setgrent_or_die();
    for (;;)
    {
        struct group    *gr;

        gr = explain_getgrent_or_die();
        if (!gr)
            break;
        printf("<group>\n");
        printf("    <name>%s</name>\n", gr->gr_name);
        printf("    <passwd>%s</passwd>\n", gr->gr_passwd);
        printf("    <gid>%d</gid>\n", gr->gr_gid);
        if (!gr->gr_mem[0])
        {
            printf("    </members>\n");
        }
        else
        {
            int             j;

            printf("    <members>\n");
            for (j = 0; gr->gr_mem[j]; ++j)
                printf("        <member>%s</member>\n", gr->gr_mem[j]);
            printf("    </members>\n");
        }
        printf("</group>\n");
    }
    explain_endgrent_or_die();
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
