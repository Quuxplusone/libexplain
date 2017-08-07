/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2010 Peter Miller
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

#include <libexplain/ac/dirent.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/closedir.h>
#include <libexplain/strtol.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/closedir.h>


void
explain_syscall_closedir(int errnum, int argc, char **argv)
{
    DIR             *dir;

    if (argc != 1)
    {
        fprintf(stderr, "closedir: requires 1 argument, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
    dir = (DIR *)explain_strtol_or_die(argv[0], 0, 0);

    explain_wrap_and_print
    (
        stdout,
        explain_errno_closedir(errnum, dir)
    );
}
