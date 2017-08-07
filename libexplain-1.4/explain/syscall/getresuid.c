/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2012, 2013 Peter Miller
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

#include <libexplain/getresuid.h>
#include <libexplain/wrap_and_print.h>
#include <libexplain/string_to_thing.h>

#include <explain/syscall/getresuid.h>


void
explain_syscall_getresuid(int errnum, int argc, char **argv)
{
    uid_t           *ruid;
    uid_t           *euid;
    uid_t           *suid;

    if (argc != 3)
    {
        fprintf(stderr, "getresuid: requires 3 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
    ruid = explain_parse_pointer_or_die(argv[0]);
    euid = explain_parse_pointer_or_die(argv[1]);
    suid = explain_parse_pointer_or_die(argv[2]);

    explain_wrap_and_print(stdout, explain_errno_getresuid(errnum, ruid, euid,
        suid));
}


/* vim: set ts=8 sw=4 et : */
