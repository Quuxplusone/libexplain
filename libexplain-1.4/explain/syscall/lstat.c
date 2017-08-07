/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/sys/stat.h>

#include <libexplain/lstat.h>
#include <libexplain/strtol.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/lstat.h>


void
explain_syscall_lstat(int errnum, int argc, char **argv)
{
    const char      *path;
    struct stat     *buf;
    struct stat     st;

    path = 0;
    buf = &st;
    switch (argc)
    {
    case 0:
        fprintf(stderr, "lstat: no path given\n");
        exit(EXIT_FAILURE);

    case 2:
        buf = (void *)explain_strtol_or_die(argv[1], 0, 0);
        /* fall through... */

    case 1:
        path = argv[0];
        break;

    default:
        fprintf(stderr, "lstat: too many arguments given\n");
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print
    (
        stdout,
        explain_errno_lstat(errnum, path, buf)
    );
}
