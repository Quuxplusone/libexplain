/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2012, 2013 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBEXPLAIN_BUFFER_ERRNO_SETUID_H
#define LIBEXPLAIN_BUFFER_ERRNO_SETUID_H

#include <libexplain/ac/unistd.h>

#include <libexplain/string_buffer.h>

/**
  * The explain_buffer_errno_setuid function is used to obtain an
  * explanation of an error returned by the <i>setuid</i>(2) system call.
  * The least the message will contain is the value of
  * <tt>strerror(errnum)</tt>, but usually it will do much better, and
  * indicate the underlying cause in more detail.
  *
  * @param sb
  *     The string buffer to print the message into. If a suitable buffer
  *     is specified, this function is thread safe.
  * @param errnum
  *     The error value to be decoded, usually obtained from the errno
  *     global variable just before this function is called. This is
  *     necessary if you need to call <b>any</b> code between the system
  *     call to be explained and this function, because many libc functions
  *     will alter the value of errno.
  * @param uid
  *     The original uid, exactly as passed to the <i>setuid</i>(2) system
  *     call.
  */
void explain_buffer_errno_setuid(explain_string_buffer_t *sb, int errnum,
    uid_t uid);

/**
  * The explain_buffer_errno_setuid_explanation function is used to obtain
  * the explanation (the part after "because") of an error returned by the
  * <i>setuid</i>(2) system call (and similar).
  *
  * @param sb
  *     The string buffer to print the message into. If a suitable buffer
  *     is specified, this function is thread safe.
  * @param errnum
  *     The error value to be decoded, usually obtained from the errno
  *     global variable just before this function is called. This is
  *     necessary if you need to call <b>any</b> code between the system
  *     call to be explained and this function, because many libc functions
  *     will alter the value of errno.
  * @param syscall_name
  *     The name of the offending system call.
  * @param uid
  *     The original uid, exactly as passed to the <i>setuid</i>(2) system
  *     call.
  */
void explain_buffer_errno_setuid_explanation(explain_string_buffer_t *sb, int
    errnum, const char *syscall_name, uid_t uid);

/* vim: set ts=8 sw=4 et : */
#endif /* LIBEXPLAIN_BUFFER_ERRNO_SETUID_H */
