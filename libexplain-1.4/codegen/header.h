/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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

#ifndef CODEGEN_HEADER_H
#define CODEGEN_HEADER_H

#include <libexplain/ac/stdio.h>

/**
  * The gpl_header function may be used to insert a GPL file header into
  * a file.
  *
  * @param fp
  *    The stream to print on
  * @param prefix
  *    The line prefix to be used.
  */
void gpl_header(FILE *fp, const char *prefix);

/**
  * The lgpl_header function may be used to insert an LGPL file header
  * into a file.
  *
  * @param fp
  *    The stream to print on
  * @param prefix
  *    The line prefix to be used.
  */
void lgpl_header(FILE *fp, const char *prefix);

#endif /* CODEGEN_HEADER_H */
