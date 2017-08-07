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

#include <libexplain/buffer/fildes.h>
#include <libexplain/buffer/permission_mode.h>
#include <libexplain/fchmod.h>
#include <libexplain/output.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/fchmod.h>


void
explain_syscall_fchmod(int errnum, int argc, char **argv)
{
    int             fildes;
    mode_t          mode;

    fildes = -1;
    mode = 0;
    switch (argc)
    {
    default:
        explain_output_error_and_die("fchmod: requires 2 arguments, "
            "not %d\n", argc);

    case 2:
        mode = explain_permission_mode_parse_or_die(argv[1], "arg two");
        /* Fall through... */

    case 1:
        fildes = explain_parse_fildes_or_die(argv[0], "arg one");
        break;
    }

    explain_wrap_and_print(stdout, explain_errno_fchmod(errnum, fildes, mode));
}


/* vim: set ts=8 sw=4 et : */
