/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2011, 2012 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/program_name.h>
#include <libexplain/setenv.h>
#include <libexplain/strtol.h>
#include <libexplain/system.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    const char      *prog;

    prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s <name> [ <value> ]\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    const char      *name;
    const char      *value;
    int             overwrite;

    for (;;)
    {
        int             c;

        c = getopt(argc, argv, "V");
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
    name = NULL;
    value = NULL;
    overwrite = 1;
    switch (argc - optind)
    {
    case 3:
        overwrite = explain_strtol_or_die(argv[optind + 2], 0, 0);
        /* Fall through... */

    case 2:
        value = argv[optind + 1];
        /* Fall through... */

    case 1:
        name = argv[optind];
        break;

    default:
        usage();
    }
    explain_setenv_or_die(name, value, overwrite);
    explain_system_success_or_die("env");
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
