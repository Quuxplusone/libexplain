/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2013 Peter Miller
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
#include <libexplain/ac/pwd.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/fildes.h>
#include <libexplain/buffer/fstatat_flags.h>
#include <libexplain/fchownat.h>
#include <libexplain/fstrcmp.h>
#include <libexplain/output.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/fchownat.h>


void
explain_syscall_fchownat(int errnum, int argc, char **argv)
{
    int             fildes;
    const char      *pathname;
    int             owner;
    int             group;
    int             flags;

    fildes = -1;
    pathname = 0;
    owner = -1;
    group = -1;
    flags = 0;
    switch (argc)
    {
    default:
        explain_output_error_and_die("fchownat: requires 5 arguments, "
            "not %d\n", argc);

    case 5:
        flags = explain_parse_fstatat_flags_or_die(argv[4], "arg 4");
        /* Fall through... */

    case 4:
        group = explain_parse_gid_t_or_die(argv[3]);
        /* Fall through... */

    case 3:
        owner = explain_parse_uid_t_or_die(argv[2]);
        /* Fall through... */

    case 2:
        pathname = argv[1];

        /* see if is openable */
        fildes = explain_parse_fildes_or_die(argv[0], "arg one");
        break;
    }

    explain_wrap_and_print(stdout, explain_errno_fchownat(errnum, fildes,
        pathname, owner, group, flags));
}


/* vim: set ts=8 sw=4 et : */
