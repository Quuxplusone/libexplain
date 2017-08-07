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
#include <libexplain/ac/sys/wait.h>
#include <libexplain/ac/sys/resource.h>

#include <libexplain/buffer/waitpid_options.h>
#include <libexplain/strtol.h>
#include <libexplain/wait4.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/wait4.h>


void
explain_syscall_wait4(int errnum, int argc, char **argv)
{
    int             pid;
    int             status_dummy;
    int             *status;
    int             options;
    struct rusage   rusage_dummy;
    struct rusage   *rusage;

    pid = -1;
    status_dummy = 0;
    status = &status_dummy;
    options = 0;
    rusage = &rusage_dummy;
    switch (argc)
    {
    case 4:
        rusage = (struct rusage *)explain_strtol_or_die(argv[3], 0, 0);
        /* fall through... */

    case 3:
        options =
            explain_parse_waitpid_options_or_die(argv[2], "wait4 arg 3");
        /* fall through... */

    case 2:
        status = (int *)explain_strtol_or_die(argv[1], 0, 0);
        /* fall through... */

    case 1:
        pid = explain_strtol_or_die(argv[0], 0, 0);
        break;

    default:
        fprintf(stderr, "wait4: requires 4 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print
    (
        stdout,
        explain_errno_wait4(errnum, pid, status, options, rusage)
    );
}
