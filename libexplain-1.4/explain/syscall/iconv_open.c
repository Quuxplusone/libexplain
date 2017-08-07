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
#include <libexplain/ac/iconv.h>

#include <libexplain/iconv_open.h>
#include <libexplain/output.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/iconv_open.h>


void
explain_syscall_iconv_open(int errnum, int argc, char **argv)
{
    const char      *to_code;
    const char      *from_code;

    to_code = "";
    from_code = "";
    switch (argc)
    {
    case 2:
        from_code = argv[1];
        /* Fall through... */

    case 1:
        to_code = argv[0];
        break;

    default:
        explain_output_error_and_die
        (
            "iconv_open: requires 2 arguments, not %d\n",
            argc
        );
    }

    explain_wrap_and_print(stdout, explain_errno_iconv_open(errnum, to_code,
        from_code));
}


/* vim: set ts=8 sw=4 et : */
