/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010, 2013 Peter Miller
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
#include <libexplain/ac/sys/uio.h>

#include <libexplain/malloc.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/writev.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/writev.h>


void
explain_syscall_writev(int errnum, int argc, char **argv)
{
    int             fildes;
    struct iovec    *iov;
    int             iovcnt;
    int             j;

    if (0 == (argc & 1))
    {
        fprintf(stderr, "writev: requires an odd number of arguments, not %d\n",
            argc);
        exit(EXIT_FAILURE);
    }
    iovcnt = (argc - 1) / 2;
    iov = explain_malloc_or_die(iovcnt * sizeof(*iov));
    fildes = explain_parse_int_or_die(argv[0]);
    for (j = 1; j < argc; j += 2)
    {
        struct iovec *p = iov + j / 2;
        p->iov_base = explain_parse_pointer_or_die(argv[j]);
        p->iov_len = explain_parse_size_t_or_die(argv[j + 1]);
    }

    explain_wrap_and_print(stdout, explain_errno_writev(errnum, fildes, iov,
        iovcnt));
}


/* vim: set ts=8 sw=4 et : */
