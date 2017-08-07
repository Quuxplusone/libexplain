/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2011, 2013 Peter Miller
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

#include <libexplain/buffer/open_flags.h>
#include <libexplain/mkostemp.h>
#include <libexplain/strdup.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/mkostemp.h>


void
explain_syscall_mkostemp(int errnum, int argc, char **argv)
{
    char            *templat;
    int             flags;

    if (argc != 2)
    {
        fprintf(stderr, "mkostemp: requires 2 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
    templat = explain_strdup_or_die(argv[0]);
    flags = explain_parse_open_flags_or_die(argv[1], "arg 2");
    explain_wrap_and_print
    (
        stdout,
        explain_errno_mkostemp(errnum, templat, flags)
    );
}


/* vim: set ts=8 sw=4 et : */
