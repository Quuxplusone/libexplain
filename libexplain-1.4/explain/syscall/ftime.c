/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010, 2013 Peter Miller
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
#include <libexplain/ac/time.h>
#include <libexplain/ac/sys/timeb.h>

#include <libexplain/ftime.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/ftime.h>


void
explain_syscall_ftime(int errnum, int argc, char **argv)
{
    struct timeb    *tp;

    if (argc != 1)
    {
        fprintf(stderr, "ftime: requires 1 argument, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
    tp = explain_parse_pointer_or_die(argv[0]);

    explain_wrap_and_print(stdout, explain_errno_ftime(errnum, tp));
}


/* vim: set ts=8 sw=4 et : */
