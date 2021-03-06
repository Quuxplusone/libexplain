/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2013 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/sys/resource.h>

#include <libexplain/buffer/getrusage_who.h>
#include <libexplain/parse_bits.h>


static const explain_parse_bits_table_t table[] =
{
#ifdef RUSAGE_SELF
    { "RUSAGE_SELF", RUSAGE_SELF },
#endif
#ifdef RUSAGE_CHILDREN
    { "RUSAGE_CHILDREN", RUSAGE_CHILDREN },
#endif
#ifdef RUSAGE_THREAD
    { "RUSAGE_THREAD", RUSAGE_THREAD },
#endif
};


void
explain_buffer_getrusage_who(explain_string_buffer_t *sb, int value)
{
    explain_parse_bits_print_single(sb, value, table, SIZEOF(table));
}


int
explain_parse_getrusage_who_or_die(const char *text, const char *caption)
{
    return explain_parse_bits_or_die(text, table, SIZEOF(table), caption);
}


/* vim: set ts=8 sw=4 et : */
