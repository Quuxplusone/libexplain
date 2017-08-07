/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009-2011 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/assert.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/ioctl.h> /* for ioctl() except Solaris */
#include <libexplain/ac/unistd.h> /* for ioctl() on Solaris */

#include <libexplain/iocontrol.h>
#include <libexplain/malloc.h>
#include <libexplain/string_buffer.h>

#include <ioctl-scan/report.h>


void
report_semi_auto(int fildes, int request)
{
    void            *data;
    const explain_iocontrol_t *p;
    int             n;
    size_t          data_size;

    data = 0;
    p = explain_iocontrol_find_by_number(fildes, request, data);
    assert(p);
    assert(p->name || p->print_name); /* i.e. p != &explain_iocontrol_generic */
    data_size = 0;
    if (p->data_size != NOT_A_POINTER)
    {
        assert(p->data_size > 0);
        data_size = p->data_size;
        if (!p->print_data_returned)
        {
            assert(p->file);
            assert(p->line);
            fprintf(stderr, "%s: %d: print_data_returned should not be zero\n",
                p->file, p->line - 2);
            assert(!"oops");
        }
    }
    if (data_size > 0)
    {
        data = explain_malloc_or_die(p->data_size);
        memset(data, 0, p->data_size);
    }
    n = ioctl(fildes, request, data);
    if (n >= 0)
    {
        char            name[100];
        explain_string_buffer_t name_sb;
        char            ret[30];
        char            repn[2000];
        explain_string_buffer_t repn_sb;

        explain_string_buffer_init(&name_sb, name, sizeof(name));
        explain_iocontrol_print_name(p, &name_sb, 0, fildes, request, data);
        snprintf(ret, sizeof(ret), "%d", n);
        explain_string_buffer_init(&repn_sb, repn, sizeof(repn));
        if (data)
            p->print_data_returned(p, &repn_sb, 0, fildes, request, data);
        else
            explain_string_buffer_puts(&repn_sb, "void");
        report(name, ret, repn);
    }
    if (data)
        free(data);
}
