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

#include <libexplain/ac/limits.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/gethostname.h>
#include <libexplain/host_name_max.h>
#include <libexplain/malloc.h>
#include <libexplain/strtol.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/gethostname.h>


void
explain_syscall_gethostname(int errnum, int argc, char **argv)
{
    char            *data;
    size_t          data_size;

    data_size = explain_get_host_name_max() + 1;
    data = explain_malloc_or_die(data_size);
    switch (argc)
    {
    case 0:
        break;

    case 1:
        data_size = explain_strtol_or_die(argv[0], 0, 0);
        break;

    case 2:
        data = (void *)explain_strtol_or_die(argv[0], 0, 0);
        data_size = explain_strtol_or_die(argv[1], 0, 0);
        break;

    default:
        fprintf(stderr, "gethostname: requires 2 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print
    (
        stdout,
        explain_errno_gethostname(errnum, data, data_size)
    );
}

/* vim: set ts=8 sw=4 et */
