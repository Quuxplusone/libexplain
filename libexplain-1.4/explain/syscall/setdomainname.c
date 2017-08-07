/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010, 2013 Peter Miller
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
#include <libexplain/ac/string.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/setdomainname.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/setdomainname.h>


void
explain_syscall_setdomainname(int errnum, int argc, char **argv)
{
    const char      *data;
    size_t          data_size;

    data = 0;
    data_size = 0;
    switch (argc)
    {
    case 1:
        data = argv[0];
        data_size = strlen(data);
        break;

    case 2:
        data = explain_parse_pointer_or_die(argv[0]);
        data_size = explain_parse_size_t_or_die(argv[1]);
        break;

    default:
        fprintf(stderr, "setdomainname: requires 2 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print(stdout, explain_errno_setdomainname(errnum, data,
        data_size));
}


/* vim: set ts=8 sw=4 et : */
