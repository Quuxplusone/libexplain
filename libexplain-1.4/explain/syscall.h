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

#ifndef EXPLAIN_SYSCALL_H
#define EXPLAIN_SYSCALL_H

typedef void (*func_t)(int err, int argc, char **argv);

/**
  * The find_function function is used to
  * locate a libc system call by name.
  *
  * @param name
  *    The name of the function to look for.
  * @returns
  *    pointer to explainer function.
  */
func_t find_function(const char *name);

/**
  * The syscall_statistics function is used to obtain statistics about
  * the number of supported system calls.  Only useful for bragging,
  * really, because it give no indication of the number left to be done.
  */
void syscall_statistics(int *total);

#endif /* EXPLAIN_SYSCALL_H */
