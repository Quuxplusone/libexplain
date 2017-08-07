/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009-2013 Peter Miller
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

#include <libexplain/ac/linux/types.h> /* old Linux revs need this */
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/sys/types.h>
#include <libexplain/ac/ustat.h>

#include <libexplain/ustat.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/ustat.h>


#ifndef HAVE_USTAT

struct ustat
{
    int dummy;
};

#endif


void
explain_syscall_ustat(int errnum, int argc, char **argv)
{
    dev_t           dev;
    struct ustat    ubuf;
    struct ustat    *ubuf_p;
    struct stat     st;

    dev = 0;
    ubuf_p = &ubuf;
    switch (argc)
    {
    case 2:
        ubuf_p = explain_parse_pointer_or_die(argv[1]);
        /* fall through... */

    case 1:
        if (stat(argv[0], &st) >= 0)
            dev = st.st_dev;
        else
            dev = explain_parse_long_or_die(argv[0]);
        break;

    default:
        fprintf(stderr, "ustat: requires 2 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print(stdout, explain_errno_ustat(errnum, dev, ubuf_p));
}


/* vim: set ts=8 sw=4 et : */
