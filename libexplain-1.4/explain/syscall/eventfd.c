/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2013, 2014 Peter Miller
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

#include <libexplain/eventfd.h>
#include <libexplain/buffer/eventfd_flags.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/eventfd.h>


void
explain_syscall_eventfd(int errnum, int argc, char **argv)
{
    unsigned int    initval;
    int             flags;

    initval = 0;
    flags = 0;
    switch (argc)
    {
    case 2:
        flags = explain_parse_eventfd_flags_or_die(argv[1], "arg 2");
        /* fall through... */

    case 1:
        initval = explain_parse_uint_or_die(argv[0]);
        /* fall through... */

    case 0:
        break;

    default:
        fprintf(stderr, "eventfd: requires 2 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print(stdout, explain_errno_eventfd(errnum, initval,
        flags));
}


/* vim: set ts=8 sw=4 et : */
