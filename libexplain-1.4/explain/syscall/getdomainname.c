/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2013 Peter Miller
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

#include <libexplain/ac/limits.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/getdomainname.h>
#include <libexplain/host_name_max.h>
#include <libexplain/malloc.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/getdomainname.h>


void
explain_syscall_getdomainname(int errnum, int argc, char **argv)
{
    char            *data_p;
    size_t          data_size;

    data_size = explain_get_host_name_max() + 1;
    data_p = explain_malloc_or_die(data_size);
    switch (argc)
    {
    case 2:
        data_size = explain_parse_size_t_or_die(argv[1]);
        /* fall through... */

    case 1:
        data_p = explain_parse_pointer_or_die(argv[0]);
        /* fall through... */

    case 0:
        break;

    default:
        fprintf(stderr, "getdomainname: requires 2 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print(stdout, explain_errno_getdomainname(errnum, data_p,
        data_size));
}


/* vim: set ts=8 sw=4 et : */
