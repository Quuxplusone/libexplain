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

#include <libexplain/buffer/prio_which.h>
#include <libexplain/setpriority.h>
#include <libexplain/output.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/setpriority.h>


void
explain_syscall_setpriority(int errnum, int argc, char **argv)
{
    int             which;
    int             who;
    int             prio;

    which = 0;
    who = 0;
    prio = 0;
    switch (argc)
    {
    default:
        explain_output_error_and_die("setpriority: requires 3 arguments, "
            "not %d\n", argc);
    case 3:
        prio = explain_parse_int_or_die(argv[2]);
        /* Fall through... */

    case 2:
        who = explain_parse_int_or_die(argv[1]);
        /* Fall through... */

    case 1:
        which = explain_parse_prio_which_or_die(argv[0], "arg 1");
        break;
    }

    explain_wrap_and_print(stdout, explain_errno_setpriority(errnum, which, who,
        prio));
}


/* vim: set ts=8 sw=4 et : */
