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
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/sys/utsname.h>

#include <libexplain/uname.h>
#include <libexplain/output.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/uname.h>


void
explain_syscall_uname(int errnum, int argc, char **argv)
{
    struct utsname  *data_ptr;
    struct utsname  data;

    data_ptr = &data;
    switch (argc)
    {
    default:
        explain_output_error_and_die("uname: requires 1 argument, not %d\n",
            argc);
        /*NOTREACHED*/

    case 1:
        data_ptr = explain_parse_pointer_or_die(argv[0]);
        break;

    case 0:
        break;
    }

    explain_wrap_and_print(stdout, explain_errno_uname(errnum, data_ptr));
}


/* vim: set ts=8 sw=4 et : */
