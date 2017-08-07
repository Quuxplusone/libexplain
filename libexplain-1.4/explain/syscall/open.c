/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009, 2013 Peter Miller
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

#include <libexplain/buffer/open_flags.h>
#include <libexplain/buffer/permission_mode.h>
#include <libexplain/open.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/open.h>


void
explain_syscall_open(int errnum, int argc, char **argv)
{
    const char      *path = 0;
    int             flags = O_RDONLY;
    int             mode = 0666;

    switch (argc)
    {
    case 3:
        mode = explain_permission_mode_parse_or_die(argv[2], "open arg 3");
        /* fall through... */

    case 2:
        flags = explain_parse_open_flags_or_die(argv[1], "open arg 2");
        /* fall through... */

    case 1:
        path = argv[0];
        break;

    default:
        fprintf(stderr, "open: needs 3 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print
    (
        stdout,
        explain_errno_open(errnum, path, flags, mode)
    );
}


/* vim: set ts=8 sw=4 et : */
