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

#ifndef CODEGEN_GET_USER_NAME_H
#define CODEGEN_GET_USER_NAME_H

/**
  * The get_user_name function may be used to extract the executing
  * user's name form the password file.
  *
  * @returns
  *    pointer to static buffer
  */
const char *get_user_name(void);

/**
  * The get_year function may be used to obtain the number, including
  * the century, of the current year.
  */
int get_year(void);

#endif /* CODEGEN_GET_USER_NAME_H */
