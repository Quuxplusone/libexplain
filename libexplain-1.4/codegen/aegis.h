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

#ifndef CODEGEN_AEGIS_H
#define CODEGEN_AEGIS_H

/**
  * The aegis_new_file function may be used to ask Aegis to create a new
  * file.
  *
  * @param filename
  *    The name of the file to be created.
  */
void aegis_new_file(const char *filename);

/**
  * The aegis_copy_file function may be used to ask Aegis to copy
  * (check out) a file for modification.
  *
  * @param filename
  *    The name of the file to be modified.
  */
void aegis_copy_file(const char *filename);

#endif /* CODEGEN_AEGIS_H */
