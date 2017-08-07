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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/termios.h>

#include <libexplain/buffer/fildes.h>
#include <libexplain/buffer/tcsetattr_options.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/tcsetattr.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/tcsetattr.h>


void
explain_syscall_tcsetattr(int errnum, int argc, char **argv)
{
    int             fildes;
    int             options;
    struct termios  data;
    struct termios  *data_p;

    fildes = 0;
    options = 0;
    memset(&data, 0, sizeof(data));
    data_p = &data;
    switch (argc)
    {
    case 3:
        data_p = explain_parse_pointer_or_die(argv[2]);
        /* fall through... */

    case 2:
        options = explain_parse_tcsetattr_options_or_die(argv[1], "argv[1]");
        /* fall through... */

    case 1:
        fildes = explain_parse_fildes_or_die(argv[0], "arg one");
        break;

    default:
        fprintf(stderr, "tcsetattr: requires 3 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print
    (
        stdout,
        explain_errno_tcsetattr(errnum, fildes, options, data_p)
    );
}


/* vim: set ts=8 sw=4 et : */
