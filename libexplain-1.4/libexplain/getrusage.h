/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2013 Peter Miller
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

#ifndef LIBEXPLAIN_GETRUSAGE_H
#define LIBEXPLAIN_GETRUSAGE_H

/**
  * @file
  * @brief explain getrusage(2) errors
  *
  * These functions may be used to obtain explanations for errors returned
  * by the <i>getrusage</i>(2) system call.
  */

#include <libexplain/gcc_attributes.h>
#include <libexplain/large_file_support.h>

#ifdef __cplusplus
extern "C" {
#endif

struct rusage; /* forward */

/**
  * The explain_getrusage_or_die function is used to call the
  * <i>getrusage</i>(2) system call. On failure an explanation will be
  * printed to stderr, obtained from the <i>#explain_getrusage</i>(3)
  * function, and then the process terminates by calling
  * <tt>exit(EXIT_FAILURE)</tt>.
  *
  * @param who
  *     The who, exactly as to be passed to the <i>getrusage</i>(2) system
  *     call.
  * @param usage
  *     The usage, exactly as to be passed to the <i>getrusage</i>(2)
  *     system call.
  * @returns
  *     This function only returns on success, see <i>getrusage</i>(2) for
  *     more information. On failure, prints an explanation and exits, it
  *     does not return.
  *
  * @par Example:
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * explain_getrusage_or_die(who, usage);
  * @endcode
  */
void explain_getrusage_or_die(int who, struct rusage *usage);

/**
  * The explain_getrusage_on_error function is used to call the
  * <i>getrusage</i>(2) system call. On failure an explanation will be
  * printed to stderr, obtained from the <i>#explain_getrusage</i>(3)
  * function.
  *
  * @param who
  *     The who, exactly as to be passed to the <i>getrusage</i>(2) system
  *     call.
  * @param usage
  *     The usage, exactly as to be passed to the <i>getrusage</i>(2)
  *     system call.
  * @returns
  *     The value returned by the wrapped <i>getrusage</i>(2) system call.
  *
  * @par Example:
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * if (explain_getrusage_on_error(who, usage) < 0)
  * {
  *     ...cope with error
  *     ...no need to print error message
  * }
  * @endcode
  */
int explain_getrusage_on_error(int who, struct rusage *usage);

/**
  * The explain_getrusage function is used to obtain an explanation of an
  * error returned by the <i>getrusage</i>(2) system call. The least the
  * message will contain is the value of <tt>strerror(errno)</tt>, but
  * usually it will do much better, and indicate the underlying cause in
  * more detail.
  *
  * The errno global variable will be used to obtain the error value to be
  * decoded.
  *
  * @param who
  *     The original who, exactly as passed to the <i>getrusage</i>(2)
  *     system call.
  * @param usage
  *     The original usage, exactly as passed to the <i>getrusage</i>(2)
  *     system call.
  * @returns
  *     The message explaining the error. This message buffer is shared by
  *     all libexplain functions which do not supply a buffer in their
  *     argument list. This will be overwritten by the next call to any
  *     libexplain function which shares this buffer, including other
  *     threads.
  * @note
  *     This function is <b>not</b> thread safe, because it shares a return
  *     buffer across all threads, and many other functions in this
  *     library.
  *
  * @par Example:
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * if (getrusage(who, usage) < 0)
  * {
  *     fprintf(stderr, "%s\n", explain_getrusage(who, usage));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  * @par
  * The above code example is available pre\-packaged as the
  * #explain_getrusage_or_die function.
  */
const char *explain_getrusage(int who, struct rusage *usage)
                                                  LIBEXPLAIN_WARN_UNUSED_RESULT;

/**
  * The explain_errno_getrusage function is used to obtain an explanation
  * of an error returned by the <i>getrusage</i>(2) system call. The least
  * the message will contain is the value of <tt>strerror(errnum)</tt>, but
  * usually it will do much better, and indicate the underlying cause in
  * more detail.
  *
  * @param errnum
  *     The error value to be decoded, usually obtained from the errno
  *     global variable just before this function is called. This is
  *     necessary if you need to call <b>any</b> code between the system
  *     call to be explained and this function, because many libc functions
  *     will alter the value of errno.
  * @param who
  *     The original who, exactly as passed to the <i>getrusage</i>(2)
  *     system call.
  * @param usage
  *     The original usage, exactly as passed to the <i>getrusage</i>(2)
  *     system call.
  * @returns
  *     The message explaining the error. This message buffer is shared by
  *     all libexplain functions which do not supply a buffer in their
  *     argument list. This will be overwritten by the next call to any
  *     libexplain function which shares this buffer, including other
  *     threads.
  * @note
  *     This function is <b>not</b> thread safe, because it shares a return
  *     buffer across all threads, and many other functions in this
  *     library.
  *
  * @par Example:
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * if (getrusage(who, usage) < 0)
  * {
  *     int err = errno;
  *     fprintf(stderr, "%s\n", explain_errno_getrusage(err, who, usage));
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  * @par
  * The above code example is available pre-packaged as the
  * #explain_getrusage_or_die function.
  */
const char *explain_errno_getrusage(int errnum, int who, struct rusage *usage)
                                                  LIBEXPLAIN_WARN_UNUSED_RESULT;

/**
  * The explain_message_getrusage function is used to obtain an explanation
  * of an error returned by the <i>getrusage</i>(2) system call. The least
  * the message will contain is the value of <tt>strerror(errnum)</tt>, but
  * usually it will do much better, and indicate the underlying cause in
  * more detail.
  *
  * The errno global variable will be used to obtain the error value to be
  * decoded.
  *
  * @param message
  *     The location in which to store the returned message. If a suitable
  *     message return buffer is supplied, this function is thread safe.
  * @param message_size
  *     The size in bytes of the location in which to store the returned
  *     message.
  * @param who
  *     The original who, exactly as passed to the <i>getrusage</i>(2)
  *     system call.
  * @param usage
  *     The original usage, exactly as passed to the <i>getrusage</i>(2)
  *     system call.
  *
  * @par Example:
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * if (getrusage(who, usage) < 0)
  * {
  *     char message[3000];
  *     explain_message_getrusage(message, sizeof(message), who, usage);
  *     fprintf(stderr, "%s\n", message);
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  * @par
  * The above code example is available pre-packaged as the
  * #explain_getrusage_or_die function.
  */
void explain_message_getrusage(char *message, int message_size, int who, struct
    rusage *usage);

/**
  * The explain_message_errno_getrusage function is used to obtain an
  * explanation of an error returned by the <i>getrusage</i>(2) system
  * call. The least the message will contain is the value of
  * <tt>strerror(errnum)</tt>, but usually it will do much better, and
  * indicate the underlying cause in more detail.
  *
  * @param message
  *     The location in which to store the returned message. If a suitable
  *     message return buffer is supplied, this function is thread safe.
  * @param message_size
  *     The size in bytes of the location in which to store the returned
  *     message.
  * @param errnum
  *     The error value to be decoded, usually obtained from the errno
  *     global variable just before this function is called. This is
  *     necessary if you need to call <b>any</b> code between the system
  *     call to be explained and this function, because many libc functions
  *     will alter the value of errno.
  * @param who
  *     The original who, exactly as passed to the <i>getrusage</i>(2)
  *     system call.
  * @param usage
  *     The original usage, exactly as passed to the <i>getrusage</i>(2)
  *     system call.
  *
  * @par Example:
  * This function is intended to be used in a fashion similar to the
  * following example:
  * @code
  * if (getrusage(who, usage) < 0)
  * {
  *     int err = errno;
  *     char message[3000];
  *     explain_message_errno_getrusage(message, sizeof(message), err, who,
  *         usage);
  *     fprintf(stderr, "%s\n", message);
  *     exit(EXIT_FAILURE);
  * }
  * @endcode
  * @par
  * The above code example is available pre-packaged as the
  * #explain_getrusage_or_die function.
  */
void explain_message_errno_getrusage(char *message, int message_size, int
    errnum, int who, struct rusage *usage);

#ifdef __cplusplus
}
#endif

/* vim: set ts=8 sw=4 et : */
#endif /* LIBEXPLAIN_GETRUSAGE_H */
