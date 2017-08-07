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

#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>

#include <libexplain/buffer/open_flags.h>
#include <libexplain/buffer/permission_mode.h>
#include <libexplain/openat.h>
#include <libexplain/open.h>
#include <libexplain/output.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/openat.h>


static int
is_a_number(const char *text)
{
    char *ep = 0;
    if (strtol(text, &ep, 0))
    {
    }
    return (ep != text && *ep == '\0');
}


void
explain_syscall_openat(int errnum, int argc, char **argv)
{
    int             fildes;
    const char      *pathname;
    int             flags;
    mode_t          mode;

    fildes = 0;
    pathname = 0;
    flags = 0;
    mode = 0;
    switch (argc)
    {
    case 4:
        mode = explain_permission_mode_parse_or_die(argv[3], "openat arg 4");
        /* Fall through... */

    case 3:
        flags = explain_parse_open_flags_or_die(argv[2], "openat arg 3");
        /* Fall through... */

    case 2:
        pathname = argv[1];
        /* Fall through... */

    case 1:
        if (is_a_number(argv[0]))
            fildes = explain_parse_int_or_die(argv[0]);
        else if (0 == strcmp(argv[0], "AT_FDCWD"))
            fildes = AT_FDCWD;
        else
            fildes = explain_open_or_die(argv[0], O_RDONLY, 0);
        break;

    default:
        explain_output_error_and_die
        (
            "openat: requires 4 arguments, not %d\n",
            argc
        );
        break;
    }

    explain_wrap_and_print(stdout, explain_errno_openat(errnum, fildes,
        pathname, flags, mode));
}


/* vim: set ts=8 sw=4 et : */
