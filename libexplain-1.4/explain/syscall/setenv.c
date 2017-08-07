/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010, 2013 Peter Miller
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

#include <libexplain/setenv.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/setenv.h>


void
explain_syscall_setenv(int errnum, int argc, char **argv)
{
    const char      *name;
    const char      *value;
    int             overwrite;

    name = 0;
    value = 0;
    overwrite = 1;
    switch (argc)
    {
    default:
        fprintf(stderr, "setenv: requires 3 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);

    case 3:
        overwrite = explain_parse_bool_or_die(argv[2]);
        /* Fall through... */

    case 2:
        value = argv[1];
        /* Fall through... */

    case 1:
        name = argv[0];
        break;
    }

    explain_wrap_and_print(stdout, explain_errno_setenv(errnum, name, value,
        overwrite));
}


/* vim: set ts=8 sw=4 et : */
