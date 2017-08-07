/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/config.h> /* for large file support */
#include <libexplain/libexplain.h>
#include <stdlib.h>
#include <unistd.h>

static void
process(FILE *fp)
{
    for (;;)
    {
        char buffer[4096];
        size_t n = explain_fread_or_die(buffer, 1, sizeof(buffer), fp);
        if (!n)
            break;
        explain_fwrite_or_die(buffer, 1, n, stdout);
    }
}

int
main(int argc, char **argv)
{
    for (;;)
    {
        int c = getopt(argc, argv, "o:V");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'o':
            explain_freopen_or_die(optarg, "w", stdout);
            break;
        case 'V':
            /* version stub, ignore */
            return 0;
        default:
            fprintf(stderr, "Usage: %ss [ -o <filename> ] "
                "<filename>...\n", argv[0]);
            return EXIT_FAILURE;
        }
    }
    if (optind == argc)
        process(stdin);
    else
    {
        while (optind < argc)
        {
            FILE *fp = explain_fopen_or_die(argv[optind++], "r");
            process(fp);
            explain_fclose_or_die(fp);
        }
    }
    explain_fflush_or_die(stdout);
    return EXIT_SUCCESS;
}
