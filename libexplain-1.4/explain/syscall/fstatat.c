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
#include <libexplain/ac/sys/stat.h>

#include <libexplain/buffer/fstatat_flags.h>
#include <libexplain/fstatat.h>
#include <libexplain/open.h>
#include <libexplain/output.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/fstatat.h>


void
explain_syscall_fstatat(int errnum, int argc, char **argv)
{
    int             fildes;
    const char      *pathname;
    struct stat     data_var;
    struct stat     *data;
    int             flags;

    fildes = 0;
    pathname = 0;
    data = &data_var;
    flags = 0;
    /* (fildes, pathnam, [ data, ] flags) */
    switch (argc)
    {
    default:
        explain_output_error_and_die("fstatat: requires 4 arguments,"
            "not %d\n", argc);
        break;

    case 4:
        flags = explain_parse_fstatat_flags_or_die(argv[3], "arg 4");
        data = explain_parse_pointer_or_die(argv[2]);
        goto arg2;

    case 3:
        flags = explain_parse_fstatat_flags_or_die(argv[2], "arg 3");
        /* Fall through... */

    case 2:
        arg2:
        pathname = argv[1];
        /* Fall through... */

    /* case 1: */
        {
            char *ep = 0;
            fildes = strtol(argv[0], &ep, 0);
            if (ep == argv[0] || *ep)
            {
                fildes = explain_open_or_die(argv[0], 0, 0);
            }
        }
        break;
    }

    explain_wrap_and_print(stdout, explain_errno_fstatat(errnum, fildes,
        pathname, data, flags));
}


/* vim: set ts=8 sw=4 et : */
