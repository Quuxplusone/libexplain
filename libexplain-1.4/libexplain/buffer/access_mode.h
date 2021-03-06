/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009, 2013 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBEXPLAIN_BUFFER_ACCESS_MODE_H
#define LIBEXPLAIN_BUFFER_ACCESS_MODE_H

struct explain_string_buffer_t; /* forward */

/**
  * The explain_buffer_access_mode function may be used to
  * emit a human readable representation of an access mode.
  *
  * @param sb
  *     The string buffer to print into.
  * @param mode
  *     The access(2) mode to encode.
  */
void explain_buffer_access_mode(struct explain_string_buffer_t *sb,
    int mode);

/**
  * The explain_access_mode_parse function may be used to parse a C-like
  * expression string, into an access(2) mode argument.
  *
  * @param text
  *     The text to be parsed.
  * @returns
  *     int; access mode on success, -1 on failure
  */
int explain_access_mode_parse(const char *text);

/**
  * The explain_access_mode_parse_or_die function may be used to
  * parse a C-like expression string, into an access(2) mode argument.
  * If string is not valie, issues a diagnostic and exits EXIT_FAILURE.
  *
  * @param text
  *     The text to be parsed.
  * @param caption
  *     Caption for error message, or NULL for none.
  * @returns
  *     the access mode
  */
int explain_access_mode_parse_or_die(const char *text, const char *caption);

#endif /* LIBEXPLAIN_BUFFER_ACCESS_MODE_H */
/* vim: set ts=8 sw=4 et : */
