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

#include <libexplain/buffer/fildes.h>
#include <libexplain/buffer/utimensat_flags.h>
#include <libexplain/linkat.h>
#include <libexplain/output.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/linkat.h>


void
explain_syscall_linkat(int errnum, int argc, char **argv)
{
    int             old_fildes;
    const char      *old_path;
    int             new_fildes;
    const char      *new_path;
    int             flags;

    old_fildes = -1;
    old_path = NULL;
    new_fildes = -1;
    new_path = NULL;
    flags = 0;

    switch (argc)
    {
    default:
        explain_output_error_and_die("linkat: requires 5 arguments, "
            "not %d\n", argc);

    case 5:
        flags = explain_parse_utimensat_flags_or_die(argv[4], "arg five");
        /* Fall through... */

    case 4:
        new_path = argv[3];
        new_fildes = explain_parse_fildes_or_die(argv[2], "arg three");
        old_path = argv[1];
        old_fildes = explain_parse_fildes_or_die(argv[0], "arg one");
        break;
    }

    explain_wrap_and_print(stdout, explain_errno_linkat(errnum, old_fildes,
        old_path, new_fildes, new_path, flags));
}


/* vim: set ts=8 sw=4 et : */
