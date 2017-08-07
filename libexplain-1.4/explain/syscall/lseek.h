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

#ifndef EXPLAIN_LSEEK_H
#define EXPLAIN_LSEEK_H

/**
  * The explain_syscall_lseek function may be used to interpret a lseek "call"
  * from the command line, and then run it through explain_syscall_errno_lseek
  * for an explanation.
  *
  * @note
  * Because the event being explain happened in another process, the
  * results are not as good as if libexplain had been used in the
  * process.
  *
  * @param errnum
  *    The number of the rrro to be explained
  * @param argc
  *    the number of command line arguments, corresponding 1:1 to the
  *    call arguments
  * @param argv
  *    the actual command line arguments, corresponding 1:1 to the call
  *    arguments
  */
void explain_syscall_lseek(int errnum, int argc, char **argv);

#endif /* EXPLAIN_LSEEK_H */
