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
#include <libexplain/ac/sys/acl.h>

#include <libexplain/acl_get_file.h>
#include <libexplain/buffer/acl_type.h>
#include <libexplain/buffer/errno/acl_get_file.h>
#include <libexplain/output.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/acl_get_file.h>


void
explain_syscall_acl_get_file(int errnum, int argc, char **argv)
{
    const char      *pathname;
    acl_type_t      type;

    if (argc != 2)
    {
        explain_output_error_and_die
        (
            "acl_get_file: requires 2 arguments, not %d\n",
            argc
        );
    }
    pathname = argv[0];
    type = explain_acl_type_parse_or_die(argv[1], "arg 2");

    explain_wrap_and_print(stdout, explain_errno_acl_get_file(errnum, pathname,
        type));
}


/* vim: set ts=8 sw=4 et : */
