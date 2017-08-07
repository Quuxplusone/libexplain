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

#include <libexplain/freopen.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/freopen.h>


void
explain_syscall_freopen(int errnum, int argc, char **argv)
{
    const char      *pathname;
    const char      *flags;
    FILE            *fp;

    if (argc != 3)
    {
        fprintf(stderr, "freopen: requires 3 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
    pathname = argv[0];
    flags = argv[1];
    fp = explain_parse_stream_or_die(argv[2], "r");

    explain_wrap_and_print(stdout, explain_errno_freopen(errnum, pathname,
        flags, fp));
}


/* vim: set ts=8 sw=4 et : */
