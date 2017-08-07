/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2011, 2012 Peter Miller
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

#include <libexplain/fclose.h>
#include <libexplain/fgets.h>
#include <libexplain/fopen.h>
#include <libexplain/output.h>
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


static int number_of_errors;


static void
check(const char *filename)
{
    FILE *fp = explain_fopen_or_die(filename, "r");
    int linum = 0;
    int saw_eacces = 0;
    int saw_eperm = 0;
    int saw_test_config_not_root = 0;
    for (;;)
    {
        char            *cp;
        char            line[1000];

        if (!explain_fgets_or_die(line, sizeof(line), fp))
            break;
        ++linum;
        if (strstr(line, "EACCES"))
            saw_eacces = linum;
        if (strstr(line, "EPERM"))
            saw_eperm = linum;
        if (strstr(line, "test_config not-root"))
            saw_test_config_not_root = linum;
        cp = strstr(line, "fmt -w");
        if (cp && cp[6] != ' ')
        {
            explain_output_error
            (
                "%s: %d: Solaris needs a space after fmt -w",
                filename,
                linum
            );
            ++number_of_errors;
        }
        if (0 == memcmp(line, "diff -", 6))
        {
            explain_output_error
            (
                "%s: %d: far too many diff options are not portable, use the "
                    "default format",
                filename,
                linum
            );
            ++number_of_errors;
        }
    }
    explain_fclose_or_die(fp);
    if ((saw_eacces || saw_eperm) && !saw_test_config_not_root)
    {
        explain_output_error
        (
            "%s: %d: the EACCES and EPERM tests do not work when invoked "
                "by root, use \"test_config not-root || pass\" to cope",
            filename,
            (saw_eacces ? saw_eacces : saw_eperm)
        );
        ++number_of_errors;
    }
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
        check(argv[optind++]);
        if (optind >= argc)
            break;
    }
    if (number_of_errors > 0)
    {
        explain_output_error_and_die
        (
            "found %d error%s",
            number_of_errors,
            (number_of_errors == 1 ? "" : "s")
        );
    }

    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
