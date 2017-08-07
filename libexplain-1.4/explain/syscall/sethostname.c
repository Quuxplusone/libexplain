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
#include <libexplain/ac/string.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/sethostname.h>
#include <libexplain/strtol.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/sethostname.h>


void
explain_syscall_sethostname(int errnum, int argc, char **argv)
{
    const char      *name;
    size_t          name_size;

    name = 0;
    name_size = 0;
    switch (argc)
    {
    case 1:
        name = argv[0];
        name_size = strlen(name);
        break;

    case 2:
        name = (char *)explain_strtol_or_die(argv[0], 0, 0);
        name_size = explain_strtol_or_die(argv[1], 0, 0);
        break;

    default:
        fprintf(stderr, "sethostname: requires 2 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print
    (
        stdout,
        explain_errno_sethostname(errnum, name, name_size)
    );
}

/* vim: set ts=8 sw=4 et */
