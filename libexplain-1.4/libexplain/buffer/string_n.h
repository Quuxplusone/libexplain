/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2013 Peter Miller
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

#ifndef LIBEXPLAIN_BUFFER_STRING_N_H
#define LIBEXPLAIN_BUFFER_STRING_N_H

#include <libexplain/string_buffer.h>

/**
  * The explain_buffer_string_n function may be used to
  * print a string, with a fixed upper limit.
  *
  * @param sb
  *     The string buffer to print into.
  * @param data
  *     The string to be printed.
  * @param data_size
  *     The maximum length of the string to be printed.
  */
void explain_buffer_string_n(explain_string_buffer_t *sb,
    const char *data, size_t data_size);

#endif /* LIBEXPLAIN_BUFFER_STRING_N_H */
/* vim: set ts=8 sw=4 et : */
