/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010, 2013 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/calloc.h>
#include <libexplain/wrap_and_print.h>
#include <libexplain/string_to_thing.h>

#include <explain/syscall/calloc.h>


void
explain_syscall_calloc(int errnum, int argc, char **argv)
{
    size_t          nmemb;
    size_t          size;

    switch (argc)
    {
    default:
        fprintf(stderr, "calloc: requires 2 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);

    case 1:
        nmemb = 1;
        size = explain_parse_size_t_or_die(argv[0]);
        break;

    case 2:
        nmemb = explain_parse_size_t_or_die(argv[0]);
        size = explain_parse_size_t_or_die(argv[1]);
        break;
    }

    explain_wrap_and_print(stdout, explain_errno_calloc(errnum, nmemb, size));
}


/* vim: set ts=8 sw=4 et : */
