/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010 Peter Miller
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

#ifndef CODEGEN_CATALOGUE_H
#define CODEGEN_CATALOGUE_H

#include <libexplain/ac/stddef.h>

typedef struct catalogue_data_t catalogue_data_t;
struct catalogue_data_t
{
    char            *name;
    char            *value;
};

typedef struct catalogue_t catalogue_t;
struct catalogue_t
{
    char            *function_name;

    catalogue_data_t **data;
    size_t          data_length;
    size_t          data_maximum;
};

/**
  * The catalogue_open function may be used to open the catalogue entry
  * for the named system call.
  *
  * @param function_name
  *    The name of the system call of interest.
  * @returns
  *    pointer to open catalogue data
  */
catalogue_t *catalogue_open(const char *function_name);

catalogue_t *catalogue_creat(const char *function_name);

const char *catalogue_get(catalogue_t *cp, const char *name);
int catalogue_get_bool(catalogue_t *cp, const char *name, int dflt);
int catalogue_get_int(catalogue_t *cp, const char *name, int dflt);
void catalogue_set(catalogue_t *cp, const char *name, const char *value);
void catalogue_set_int(catalogue_t *cp, const char *name, int value);
void catalogue_set_bool(catalogue_t *cp, const char *name, int value);
void catalogue_close(catalogue_t *cp);
void catalogue_save(catalogue_t *cp);

#endif /* CODEGEN_CATALOGUE_H */
