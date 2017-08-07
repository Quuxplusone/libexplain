/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2011 Peter Miller
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

#include <libexplain/ac/limits.h> /* for PATH_MAX on Solaris */
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/sys/param.h> /* for PATH_MAX except Solaris */

#include <libexplain/realpath.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/realpath.h>


void
explain_syscall_realpath(int errnum, int argc, char **argv)
{
    const char      *pathname;
    char            resolved_pathname[PATH_MAX + 1];

    if (argc != 1)
    {
        fprintf(stderr, "realpath: requires 1 argument, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
    pathname = argv[0];

    explain_wrap_and_print(stdout, explain_errno_realpath(errnum, pathname,
        resolved_pathname));
}


/* vim: set ts=8 sw=4 et */
