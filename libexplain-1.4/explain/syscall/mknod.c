/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010 Peter Miller
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
#include <libexplain/ac/sys/types.h>

#include <libexplain/buffer/dev_t.h>
#include <libexplain/buffer/stat_mode.h>
#include <libexplain/mknod.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/mknod.h>


void
explain_syscall_mknod(int errnum, int argc, char **argv)
{
    const char      *pathname;
    mode_t          mode;
    dev_t           dev;

    if (argc != 3)
    {
        fprintf(stderr, "mknod: requires 3 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
    pathname = argv[0];
    mode = explain_parse_stat_mode_or_die(argv[1], "arg 2");
    dev = explain_parse_dev_t_or_die(argv[2], "arg 3");

    explain_wrap_and_print(stdout, explain_errno_mknod(errnum, pathname, mode,
        dev));
}


/* vim: set ts=8 sw=4 et */
