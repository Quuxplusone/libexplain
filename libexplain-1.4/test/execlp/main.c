/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2012 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but
 * WITHOUT ANY WARRANTY; without even the implied warranty
 * ofMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNULesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/assert.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/execlp.h>
#include <libexplain/execvp.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_execlp\n");
    fprintf(stderr, "       test_execlp -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    char            **av;

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
    av = argv + optind;

    switch (argc - optind)
    {
    case 0:
        usage();

    case 1:
        explain_execlp_or_die(av[0], av[0], (const char *)NULL);
        break;

    case 2:
        explain_execlp_or_die(av[0], av[0], av[1], (const char *)NULL);
        break;

    case 3:
        explain_execlp_or_die(av[0], av[0], av[1], av[2], (const char *)NULL);
        break;

    case 4:
        explain_execlp_or_die(av[0], av[0], av[1], av[2], av[3],
            (const char *)NULL);
        break;

    case 5:
        explain_execlp_or_die(av[0], av[0], av[1], av[2], av[3], av[4],
            (const char *)NULL);
        break;

    case 6:
        explain_execlp_or_die(av[0], av[0], av[1], av[2], av[3], av[4], av[5],
            (const char *)NULL);
        break;

    case 7:
        explain_execlp_or_die(av[0], av[0], av[1], av[2], av[3], av[4], av[5],
            av[6], (const char *)NULL);
        break;

    case 8:
        explain_execlp_or_die(av[0], av[0], av[1], av[2], av[3], av[4], av[5],
            av[6], av[7], (const char *)NULL);
        break;

    default:
        explain_execvp_or_die(av[0], av);
        break;
    }
    assert(!"should never get here");
    return EXIT_FAILURE;
}


/* vim: set ts=8 sw=4 et : */
