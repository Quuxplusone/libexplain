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
#include <libexplain/ac/unistd.h>

#include <libexplain/access.h>
#include <libexplain/buffer/access_mode.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/access.h>


void
explain_syscall_access(int errnum, int argc, char **argv)
{
    const char      *pathname;
    int             mode;

    mode = F_OK;
    switch (argc)
    {
    case 2:
        mode = explain_access_mode_parse(argv[1]);
        if (mode < 0)
        {
            fprintf
            (
                stderr,
                "argument \"%s\" does not look like an access mode\n",
                argv[1]
            );
            exit(EXIT_FAILURE);
        }
        /* fall through... */

    case 1:
        pathname = argv[0];
        break;

    default:
        fprintf(stderr, "access: requires 2 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print
    (
        stdout,
        explain_errno_access(errnum, pathname, mode)
    );
}
