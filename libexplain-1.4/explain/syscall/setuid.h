/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2012 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EXPLAIN_SYSCALL_SETUID_H
#define EXPLAIN_SYSCALL_SETUID_H

/**
  * The explain_syscall_setuid function is used to interpret a setuid
  * "call" from the command line, and then run it through the
  * explain_errno_setuid function for an explanation.
  *
  * @note
  *     Because the event being explain happened in another process, the
  *     results are not as good as if explain_errno_setuid had been called
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
void explain_syscall_setuid(int errnum, int argc, char **argv);

/* vim: set ts=8 sw=4 et : */
#endif /* EXPLAIN_SYSCALL_SETUID_H */