/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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
#include <libexplain/ac/time.h>

#include <libexplain/buffer/time_t.h>
#include <libexplain/stime.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/stime.h>


void
explain_syscall_stime(int errnum, int argc, char **argv)
{
    time_t          t;

    if (argc != 1)
    {
        fprintf(stderr, "stime: requires 1 argument, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
    t = explain_parse_time_t_or_die(argv[0], "arg 1");

    explain_wrap_and_print(stdout, explain_errno_stime(errnum, &t));
}


/* vim: set ts=8 sw=4 et */
