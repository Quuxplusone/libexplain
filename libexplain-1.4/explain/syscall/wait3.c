/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/sys/resource.h>
#include <libexplain/ac/sys/wait.h>

#include <libexplain/buffer/waitpid_options.h>
#include <libexplain/strtol.h>
#include <libexplain/wait3.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/wait3.h>


void
explain_syscall_wait3(int errnum, int argc, char **argv)
{
    int             status_dummy;
    int             *status;
    int             options;
    struct rusage   rusage_dummy;
    struct rusage   *rusage;

    status = &status_dummy;
    rusage = &rusage_dummy;
    options = 0;
    switch (argc)
    {
    case 3:
        rusage = (struct rusage *)explain_strtol_or_die(argv[2], 0, 0);
        /* fall through... */

    case 2:
        options =
            explain_parse_waitpid_options_or_die(argv[1], "wait3 arg 2");
        /* fall through... */

    case 1:
        status = (int *)explain_strtol_or_die(argv[0], 0, 0);
        break;

    default:
        fprintf(stderr, "wait3: requires 3 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print
    (
        stdout,
        explain_errno_wait3(errnum, status, options, rusage)
    );
}
