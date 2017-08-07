/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009, 2012 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/fclose.h>
#include <libexplain/fcntl.h>
#include <libexplain/fflush.h>
#include <libexplain/fopen.h>
#include <libexplain/fputc.h>
#include <libexplain/fread.h>
#include <libexplain/fwrite.h>
#include <libexplain/putc.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_open [ <option>... ] <pathname>\n");
    fprintf(stderr, "       test_open -V\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    FILE            *fp;
    const char      *pathname;
    const char      *flags;
    int             fd_flags;

    flags = "r";
    for (;;)
    {
        int c = getopt(argc, argv, "f:V");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'f':
            flags = optarg;
            break;

        case 'V':
            explain_version_print();
            return 0;

        default:
            usage();
        }
    }
    if (optind + 1 != argc)
        usage();
    pathname = argv[optind];

    fp = explain_fopen_or_die(pathname, flags);
    explain_fflush_or_die(fp);
    fd_flags = explain_fcntl_or_die(fileno(fp), F_GETFL, 0);
    if ((fd_flags & O_ACCMODE) != O_RDONLY)
    {
        const char      *data;

        explain_putc_or_die('T', fp);
        explain_fputc_or_die('h', fp);
        data = "is is a test.\n";
        explain_fwrite_or_die(data, 1, strlen(data), fp);
    }
    if ((fd_flags & O_ACCMODE) != O_WRONLY)
    {
        char            data[100];

        explain_fread_or_die(data, 1, sizeof(data), fp);
    }
    explain_fclose_or_die(fp);
    return 0;
}


/* vim: set ts=8 sw=4 et : */
