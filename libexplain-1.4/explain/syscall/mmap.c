/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010, 2013 Peter Miller
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
#include <libexplain/ac/sys/mman.h>

#include <libexplain/buffer/mmap_flags.h>
#include <libexplain/buffer/mmap_prot.h>
#include <libexplain/mmap.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/mmap.h>


void
explain_syscall_mmap(int errnum, int argc, char **argv)
{
    void            *data;
    size_t          data_size;
    int             prot;
    int             flags;
    int             fildes;
    off_t           offset;

    offset = 0;
    switch (argc)
    {
    case 6:
        offset = explain_parse_off_t_or_die(argv[5]);
        /* Fall through... */

    case 5:
        break;

    default:
        fprintf(stderr, "mmap: requires 6 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }
    data = explain_parse_pointer_or_die(argv[0]);
    data_size = explain_parse_size_t_or_die(argv[1]);
    prot = explain_parse_mmap_prot_or_die(argv[2], "arg 3");
    flags = explain_parse_mmap_flags_or_die(argv[3], "arg 4");
    fildes = explain_parse_int_or_die(argv[4]);

    explain_wrap_and_print(stdout, explain_errno_mmap(errnum, data, data_size,
        prot, flags, fildes, offset));
}


/* vim: set ts=8 sw=4 et : */
