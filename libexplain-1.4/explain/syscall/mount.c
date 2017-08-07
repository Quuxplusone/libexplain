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

#include <libexplain/mount.h>
#include <libexplain/buffer/mount_flags.h>
#include <libexplain/output.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/mount.h>


void
explain_syscall_mount(int errnum, int argc, char **argv)
{
    const char      *source = 0;
    const char      *target = 0;
    const char      *file_system_type = 0;
    unsigned long   flags = 0;
    const void      *data = 0;

    switch (argc)
    {
    default:
        explain_output_error_and_die("mount: requires 5 arguments, "
            "not %d\n", argc);

    case 5:
        data = argv[4];
        /* Fall through... */

    case 4:
        flags = explain_parse_mount_flags_or_die(argv[3], "arg four");
        /* Fall through... */

    case 3:
        file_system_type = argv[2];
        /* Fall through... */

    case 2:
        target = argv[1];
        /* Fall through... */

    case 1:
        source = argv[0];
        break;
    }

    explain_wrap_and_print(stdout, explain_errno_mount(errnum, source, target,
        file_system_type, flags, data));
}


/* vim: set ts=8 sw=4 et : */
