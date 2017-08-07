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
#include <libexplain/ac/sys/types.h>

#include <libexplain/buffer/signal.h>
#include <libexplain/kill.h>
#include <libexplain/strtol.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/kill.h>


void
explain_syscall_kill(int errnum, int argc, char **argv)
{
    pid_t           pid;
    int             sig;

    if (argc != 2)
    {
        fprintf(stderr, "kill: requires 2 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
    pid = explain_strtol_or_die(argv[0], 0, 0);
    sig = explain_signal_parse_or_die(argv[1], "arg 2");

    explain_wrap_and_print(stdout, explain_errno_kill(errnum, pid, sig));
}


/* vim: set ts=8 sw=4 et */
