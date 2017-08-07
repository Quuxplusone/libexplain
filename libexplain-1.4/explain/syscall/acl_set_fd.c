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
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/acl.h>

#include <libexplain/acl_from_text.h>
#include <libexplain/acl_set_fd.h>
#include <libexplain/buffer/fildes.h>
#include <libexplain/output.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/acl_set_fd.h>


void
explain_syscall_acl_set_fd(int errnum, int argc, char **argv)
{
    int fildes = -1;
    acl_t acl = acl_from_text("u::rw-, g::r--, o::r--");

    switch (argc)
    {
    case 2:
        if (0 == strcmp(argv[1], "NULL"))
            acl = NULL;
        else
            acl = explain_acl_from_text_or_die(argv[1]);
        /* Fall through ... */

    case 1:
        fildes = explain_parse_fildes_or_die(argv[0], "arg one");
        break;

    default:
        explain_output_error_and_die
        (
            "acl_set_fd: requires 2 arguments, not %d\n",
            argc
        );
    }

    explain_wrap_and_print(stdout, explain_errno_acl_set_fd(errnum, fildes,
        acl));
}


/* vim: set ts=8 sw=4 et : */
