/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010 Peter Miller
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
#include <libexplain/ac/sys/timerfd.h>

#include <libexplain/buffer/clockid.h>
#include <libexplain/buffer/timerfd_flags.h>
#include <libexplain/timerfd_create.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/timerfd_create.h>


void
explain_syscall_timerfd_create(int errnum, int argc, char **argv)
{
    int             clockid;
    int             flags;

    clockid = CLOCK_MONOTONIC;
    flags = 0;
    switch (argc)
    {
    case 2:
        flags = explain_parse_timerfd_flags_or_die(argv[1], "arg 2");
        /* fall through... */

    case 1:
        clockid = explain_parse_clockid_or_die(argv[0], "arg 1");
        /* fall through... */

    case 0:
        break;

    default:
        fprintf(stderr, "timerfd_create: requires 2 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print(stdout, explain_errno_timerfd_create(errnum, clockid,
        flags));
}


/* vim: set ts=8 sw=4 et */
