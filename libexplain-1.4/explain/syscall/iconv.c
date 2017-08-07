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

#include <libexplain/ac/iconv.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/iconv.h>
#include <libexplain/output.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/iconv.h>


void
explain_syscall_iconv(int errnum, int argc, char **argv)
{
    iconv_t         cd;
    char            *inbuf;
    size_t          inbytesleft;
    char            *outbuf;
    size_t          outbytesleft;

    cd = NULL;
    inbuf = NULL;
    inbytesleft = 0;
    outbuf = NULL;
    outbytesleft = 0;
    switch(argc)
    {
    case 5:
        outbytesleft = explain_parse_size_t_or_die(argv[4]);
        /* Fall through... */

    case 4:
        outbuf = explain_parse_pointer_or_die(argv[3]);
        /* Fall through... */

    case 3:
        inbytesleft = explain_parse_size_t_or_die(argv[2]);
        /* Fall through... */

    case 2:
        inbuf = explain_parse_pointer_or_die(argv[1]);
        /* Fall through... */

    case 1:
        cd = explain_parse_pointer_or_die(argv[0]);
        break;

    default:
        explain_output_error_and_die
        (
            "iconv: requires 5 arguments, not %d\n",
            argc
        );
        break;
    }

    explain_wrap_and_print(stdout, explain_errno_iconv(errnum, cd, &inbuf,
        &inbytesleft, &outbuf, &outbytesleft));
}


/* vim: set ts=8 sw=4 et : */
