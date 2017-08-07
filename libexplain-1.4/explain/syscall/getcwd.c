/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2010 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/limits.h> /* for PATH_MAX on Solaris */
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/sys/param.h> /* for PATH_MAX except Solaris */
#include <libexplain/ac/unistd.h>

#include <libexplain/getcwd.h>
#include <libexplain/strtol.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/getcwd.h>


void
explain_syscall_getcwd(int errnum, int argc, char **argv)
{
    switch (argc)
    {
    case 0:
        {
            char            data[PATH_MAX + 1];

            explain_wrap_and_print
            (
                stdout,
                explain_errno_getcwd(errnum, data, sizeof(data))
            );
        }
        break;

    case 2:
        {
            char            *data;
            long            data_size;

            data = (char *)explain_strtol_or_die(argv[0], 0, 0);
            data_size = explain_strtol_or_die(argv[1], 0, 0);
            explain_wrap_and_print
            (
                stdout,
                explain_errno_getcwd(errnum, data, data_size)
            );
        }
        break;

    default:
        fprintf(stderr, "getcwd: requires 2 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
}
