/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/execlp.h>
#include <libexplain/execvp.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/execlp.h>


void
explain_syscall_execlp(int errnum, int argc, char **argv)
{
    switch (argc)
    {
    case 0:
        fprintf(stderr, "execlp: requires at least 1 argument\n");
        exit(EXIT_FAILURE);

    case 1:
        explain_wrap_and_print(stdout, explain_errno_execlp(errnum, argv[0],
            argv[0], NULL));
        break;

    case 2:
        explain_wrap_and_print(stdout, explain_errno_execlp(errnum, argv[0],
            argv[0], argv[1], NULL));
        break;

    case 3:
        explain_wrap_and_print(stdout, explain_errno_execlp(errnum, argv[0],
            argv[0], argv[1], argv[2], NULL));
        break;

    case 4:
        explain_wrap_and_print(stdout, explain_errno_execlp(errnum, argv[0],
            argv[0], argv[1], argv[2], argv[3], NULL));
        break;

    case 5:
        explain_wrap_and_print(stdout, explain_errno_execlp(errnum, argv[0],
            argv[0], argv[1], argv[2], argv[3], argv[4], NULL));
        break;

    case 6:
        explain_wrap_and_print(stdout, explain_errno_execlp(errnum, argv[0],
            argv[0], argv[1], argv[2], argv[3], argv[4], argv[5], NULL));
        break;

    default:
        explain_wrap_and_print
        (
            stdout,
            explain_errno_execvp(errnum, argv[0], argv)
        );
        break;
    }
}


/* vim: set ts=8 sw=4 et */
