/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2012, 2013 Peter Miller
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
#include <libexplain/ac/sys/stat.h>

#include <libexplain/lchmod.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/lchmod.h>


void
explain_syscall_lchmod(int errnum, int argc, char **argv)
{
    const char      *pathname;
    mode_t          mode;

    if (argc != 2)
    {
        fprintf(stderr, "lchmod: requires 2 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    pathname = argv[0];
    mode = explain_parse_long_or_die(argv[1]);

    explain_wrap_and_print
    (
        stdout,
        explain_errno_lchmod(errnum, pathname, mode)
    );
}


/* vim: set ts=8 sw=4 et : */
