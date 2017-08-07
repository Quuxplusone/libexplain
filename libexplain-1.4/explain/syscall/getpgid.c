/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2011 Peter Miller
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
#include <libexplain/ac/unistd.h>

#include <libexplain/getpgid.h>
#include <libexplain/strtol.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/getpgid.h>


void
explain_syscall_getpgid(int errnum, int argc, char **argv)
{
    pid_t           pid;

    if (argc != 1)
    {
        fprintf(stderr, "getpgid: requires 1 argument, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
    pid = explain_strtol_or_die(argv[0], 0, 0);

    explain_wrap_and_print(stdout, explain_errno_getpgid(errnum, pid));
}


/* vim: set ts=8 sw=4 et */
