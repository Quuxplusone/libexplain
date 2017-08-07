/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009 Peter Miller
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

#ifndef CODEGEN_GRAM_H
#define CODEGEN_GRAM_H

#include <codegen/node.h>


/**
  * The grammar function may be used to parse the given text into a tree
  * representation of a C function declaration.
  *
  * @param text
  *    The text to be parsed.
  * @returns
  *    Root of tree.
  */
node_t *grammar(const char *text);

#endif /* CODEGEN_GRAM_H */
