/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but
 * WITHOUT ANY WARRANTY; without even the implied warranty
 * ofMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNULesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EXPLAIN_SYSCALL_ACCEPT4_H
#define EXPLAIN_SYSCALL_ACCEPT4_H

/**
  * The explain_syscall_accept4 function is used to interpret a accept4
  * "call" from the command line, and then run it through the
  * explain_errno_accept4 function for an explanation.
  *
  * @note
  *     Because the event being explain happened in another process, the
  *     results are not as good as if explain_errno_accept4 had been called
  *     from within the process that saw the error.
  *
  * @param errnum
  *     The number of the error to be explained.
  * @param argc
  *     the number of command line arguments, corresponding 1:1 to the call
  *     arguments.
  * @param argv
  *     the actual command line arguments, corresponding 1:1 to the call
  *     arguments.
  */
void explain_syscall_accept4(int errnum, int argc, char **argv);

/* vim: set ts=8 sw=4 et */
#endif /* EXPLAIN_SYSCALL_ACCEPT4_H */
