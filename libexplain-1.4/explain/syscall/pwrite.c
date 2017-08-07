/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010 Peter Miller
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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/pwrite.h>
#include <libexplain/strtol.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/pwrite.h>


void
explain_syscall_pwrite(int errnum, int argc, char **argv)
{
    int             fildes;
    const void      *data;
    size_t          data_size;
    off_t           offset;

    if (argc != 4)
    {
        fprintf(stderr, "pwrite: requires 4 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
    fildes = explain_strtol_or_die(argv[0], 0, 0);
    data = (void *)explain_strtol_or_die(argv[1], 0, 0);
    data_size = explain_strtol_or_die(argv[2], 0, 0);
    offset = explain_strtol_or_die(argv[3], 0, 0);

    explain_wrap_and_print(stdout, explain_errno_pwrite(errnum, fildes, data,
        data_size, offset));
}


/* vim: set ts=8 sw=4 et */
