/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but
 * WITHOUT ANY WARRANTY; without even the implied warranty
 * ofMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNULesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/mkdtemp.h>
#include <libexplain/strdup.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/mkdtemp.h>


void
explain_syscall_mkdtemp(int errnum, int argc, char **argv)
{
    char            *pathname;

    if (argc != 1)
    {
        fprintf(stderr, "mkdtemp: requires 1 argument, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
    pathname = explain_strdup_or_die(argv[0]);

    explain_wrap_and_print(stdout, explain_errno_mkdtemp(errnum, pathname));
}


/* vim: set ts=8 sw=4 et */
