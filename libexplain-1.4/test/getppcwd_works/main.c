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
#include <libexplain/ac/unistd.h>

#include <libexplain/getppcwd.h>
#include <libexplain/program_name.h>
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


static int
lsof_available(void)
{
    return (0 == system("lsof -v >/dev/null 2>/dev/null"));
}


int
main(int argc, char **argv)
{
    char buf[2000];

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

    /*
     * This test checks to make sure that the explain_getppcwd function
     * actually works.  This means that either /proc is present and
     * working, or that lsof(1) is present an working.
     */
    if (!explain_getppcwd(buf, sizeof(buf)))
    {
        printf
        (
            "\n"
            "\tThis system is less than forthcoming when providing\n"
            "\tinformation about open files.  This does NOT mean that\n"
            "\tlibexplain's error explanations will be wrong, they will\n"
            "\tsimply be less informative than they could be.\n"
            "\n"
        );
#if (PROC_PID_CWD == 0) && (PROC_PID_PATH_CWD == 0)
        printf
        (
            "\tThe current environment does not appear have a sufficiently\n"
            "\tcapable /proc implementation.  This can be caused by chroot\n"
            "\tbuild jails that do not provide a /proc mount.\n"
            "\n"
        );
#endif
        if (!lsof_available())
        {
            printf
            (
                 "\tThe current environment does not appear to have lsof(1)\n"
                 "\tinstalled.  This can be useful when /proc is inadequate.\n"
                 "\n"
            );
        }
        printf
        (
            "\tThis test is declared to pass by default.\n"
            "\n"
        );
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
