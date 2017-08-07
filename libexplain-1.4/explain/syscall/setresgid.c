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

#include <libexplain/setresgid.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/setresgid.h>


void
explain_syscall_setresgid(int errnum, int argc, char **argv)
{
    gid_t           rgid;
    gid_t           egid;
    gid_t           sgid;

    rgid = -1;
    egid = -1;
    sgid = -1;
    switch (argc)
    {
    case 3:
        sgid = explain_parse_gid_t_or_die(argv[2]);
    case 2:
        egid = explain_parse_gid_t_or_die(argv[1]);
    case 1:
        rgid = explain_parse_gid_t_or_die(argv[0]);
    case 0:
        break;

    default:
        fprintf(stderr, "setresgid: requires 3 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print
    (
        stdout,
        explain_errno_setresgid(errnum, rgid, egid, sgid)
    );
}


/* vim: set ts=8 sw=4 et : */
