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

#include <libexplain/ac/signal.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/buffer/signalfd_flags.h>
#include <libexplain/buffer/signal.h>
#include <libexplain/signalfd.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/signalfd.h>


void
explain_syscall_signalfd(int errnum, int argc, char **argv)
{
    int             fildes;
    sigset_t        mask;
    int             flags;

    fildes = -1;
    sigemptyset(&mask);
    flags = 0;
    switch (argc)
    {
    case 3:
        flags = explain_parse_signalfd_flags_or_die(argv[2], "arg 3");
        /* fall through... */

    case 2:
        sigaddset(&mask, explain_signal_parse_or_die(argv[1], "arg 2"));
        /* fall through... */

    case 1:
        fildes = explain_parse_int_or_die(argv[0]);
        /* fall through... */

    case 0:
        break;

    default:
        fprintf(stderr, "signalfd: requires 3 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print(stdout, explain_errno_signalfd(errnum, fildes, &mask,
        flags));
}


/* vim: set ts=8 sw=4 et : */
