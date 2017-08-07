/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2010, 2013 Peter Miller
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
#include <libexplain/ac/utime.h>

#include <libexplain/string_to_thing.h>
#include <libexplain/utime.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/utime.h>


void
explain_syscall_utime(int errnum, int argc, char **argv)
{
    const char      *pathname;
    static struct utimbuf  times_zero;
    struct utimbuf  times_dummy;
    struct utimbuf  *times;

    times_dummy = times_zero;
    times = &times_dummy;
    switch (argc)
    {
    case 2:
        times = explain_parse_pointer_or_die(argv[1]);
        /* fall through... */

    case 1:
        pathname = argv[0];
        break;

    default:
        fprintf(stderr, "utime: requires 2 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print
    (
        stdout,
        explain_errno_utime(errnum, pathname, times)
    );
}


/* vim: set ts=8 sw=4 et : */
