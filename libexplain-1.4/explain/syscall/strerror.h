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

#ifndef EXPLAIN_STRERROR_H
#define EXPLAIN_STRERROR_H

/**
  * The explain_syscall_strerror function may be used to interpret an error from
  * the command line. it could be a number (e.g. 2), an error symbol
  * (e.g. ENOENT) or and error text (e.g. "No such file or directory").
  * All three pieces of information will be printed out.
  *
  * @param errnum
  *    The number of the error to be explained
  * @param argc
  *    the number of command line arguments, corresponding 1:1 to the
  *    call arguments
  * @param argv
  *    the actual command line arguments, corresponding 1:1 to the call
  *    arguments
  */
void explain_syscall_strerror(int errnum, int argc, char **argv);

#endif /* EXPLAIN_STRERROR_H */
