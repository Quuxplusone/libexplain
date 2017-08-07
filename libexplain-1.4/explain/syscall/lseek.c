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

#include <libexplain/buffer/lseek_whence.h>
#include <libexplain/lseek.h>
#include <libexplain/strtol.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/lseek.h>


void
explain_syscall_lseek(int errnum, int argc, char **argv)
{
    int fildes = -1;
    long offset = 0;
    int whence = SEEK_SET;
    switch (argc)
    {
    case 0:
        fprintf(stderr, "lseek: requires 3 arguments\n");
        exit(EXIT_FAILURE);

    case 3:
        whence = explain_lseek_whence_parse_or_die(argv[2], "lseek arg 2");
        /* fall through... */

    case 2:
        offset = explain_strtol_or_die(argv[1], 0, 0);
        /* fall through... */

    case 1:
        fildes = explain_strtol_or_die(argv[0], 0, 0);
        break;

    default:
        fprintf(stderr, "lseek: too many arguments given\n");
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print
    (
        stdout,
        explain_errno_lseek(errnum, fildes, offset, whence)
    );
}
