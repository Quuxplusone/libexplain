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

#include <libexplain/acl_from_text.h>
#include <libexplain/acl_set_file.h>
#include <libexplain/buffer/acl_type.h>
#include <libexplain/output.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/acl_set_file.h>


void
explain_syscall_acl_set_file(int errnum, int argc, char **argv)
{
    const char      *pathname;
    acl_type_t      type = ACL_TYPE_ACCESS;
    acl_t           acl = acl_from_text("u::rw- g::r-- o::r--");

    switch (argc)
    if (argc != 3)
    {
    case 3:
        acl = explain_acl_from_text_or_die(argv[2]);
        /* Fall through ... */

    case 2:
        type = explain_acl_type_parse_or_die(argv[1], "arg 2");
        /* Fall through ... */

    case 1:
        pathname = argv[0];
        break;

    default:
        explain_output_error_and_die("acl_set_file: requires 3 arguments, "
            "not %d\n", argc);
        break;
    }

    explain_wrap_and_print(stdout, explain_errno_acl_set_file(errnum, pathname,
        type, acl));
}


/* vim: set ts=8 sw=4 et : */
