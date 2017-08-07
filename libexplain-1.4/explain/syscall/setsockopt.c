/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/sys/socket.h>

#include <libexplain/buffer/sockopt_level.h>
#include <libexplain/buffer/sockopt_name.h>
#include <libexplain/setsockopt.h>
#include <libexplain/strtol.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/setsockopt.h>


void
explain_syscall_setsockopt(int errnum, int argc, char **argv)
{
    int             fildes;
    int             level;
    int             name;
    int             data;
    socklen_t       data_size;

    fildes = -1;
    level = -1;
    name = -1;
    data = 0;
    data_size = sizeof(data);
    switch (argc)
    {
    case 5:
        data_size = explain_strtol_or_die(argv[4], 0, 0);
        /* fall through... */

    case 4:
        data = explain_strtol_or_die(argv[3], 0, 0);
        /* fall through... */

    case 3:
        name =
            explain_parse_sockopt_name_or_die(argv[2], "setsockopt arg 3");
        /* fall through... */

    case 2:
        level =
            explain_parse_sockopt_level_or_die(argv[1], "setsockopt arg 2");
        /* fall through... */

    case 1:
        fildes = explain_strtol_or_die(argv[0], 0, 0);
        break;

    default:
        fprintf(stderr, "setsockopt: requires 5 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print
    (
        stdout,
        explain_errno_setsockopt
        (
            errnum,
            fildes,
            level,
            name,
            &data,
            data_size
        )
    );
}

/* vim: set ts=8 sw=4 et */
