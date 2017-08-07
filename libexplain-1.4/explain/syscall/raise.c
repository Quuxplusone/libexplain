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

#include <libexplain/buffer/signal.h>
#include <libexplain/raise.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/raise.h>


void
explain_syscall_raise(int errnum, int argc, char **argv)
{
    int             sig;

    switch (argc)
    {
    case 1:
        sig = explain_signal_parse_or_die(argv[0], "argv[0]");
        break;

    default:
        fprintf(stderr, "raise: requires 1 argument, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print(stdout, explain_errno_raise(errnum, sig));
}


/* vim: set ts=8 sw=4 et */
