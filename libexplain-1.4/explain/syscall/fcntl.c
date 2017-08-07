/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2010, 2013 Peter Miller
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

#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>

#include <libexplain/fcntl.h>
#include <libexplain/buffer/errno/fcntl.h>
#include <libexplain/buffer/open_flags.h>
#include <libexplain/buffer/signal.h>
#include <libexplain/strtol.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/fcntl.h>


void
explain_syscall_fcntl(int errnum, int argc, char **argv)
{
    int             fildes;
    int             command;

    if (argc < 2)
    {
        fprintf(stderr, "fcntl: at least 2 arguments required\n");
        exit(EXIT_FAILURE);
    }
    fildes = explain_strtol_or_die(argv[0], 0, 0);
    command = explain_fcntl_command_parse_or_die(argv[1], "fcntl arg 2");
    switch (command)
    {
    default:
        if (argc > 2)
        {
            fprintf(stderr, "fcntl: need 2 arguments (not %d)\n", argc);
            exit(EXIT_FAILURE);
        }
        explain_wrap_and_print
        (
            stdout,
            explain_errno_fcntl(errnum, fildes, command, 0)
        );
        break;

#ifdef F_SETSIG
    case F_SETSIG:
        {
            long            arg;

            if (argc != 3)
            {
                fprintf(stderr, "fcntl: need 3 arguments (not %d)\n", argc);
                exit(EXIT_FAILURE);
            }
            arg = explain_signal_parse_or_die(argv[2], "fcntl arg 3");
            explain_wrap_and_print
            (
                stdout,
                explain_errno_fcntl(errnum, fildes, command, arg)
            );
        }
        break;
#endif

#ifdef F_NOTIFY
    case F_NOTIFY: /* FIXME: parse notify bits */
#endif
#ifdef F_SETLEASE
    case F_SETLEASE: /* FIXME: parse lease bits */
#endif
    case F_DUPFD:
#ifdef F_DUPFD_CLOEXEC
    case F_DUPFD_CLOEXEC:
#endif
    case F_SETFD:
    case F_SETOWN:
        {
            long            arg;

            if (argc != 3)
            {
                fprintf(stderr, "fcntl: need 3 arguments (not %d)\n", argc);
                exit(EXIT_FAILURE);
            }
            arg = explain_strtol_or_die(argv[2], 0, 0);
            explain_wrap_and_print
            (
                stdout,
                explain_errno_fcntl(errnum, fildes, command, arg)
            );
        }
        break;

    case F_SETFL:
        {
            long            arg;

            if (argc != 3)
            {
                fprintf(stderr, "fcntl: need 3 arguments (not %d)\n", argc);
                exit(EXIT_FAILURE);
            }
            arg = explain_parse_open_flags_or_die(argv[2], "fcntl arg 3");
            explain_wrap_and_print
            (
                stdout,
                explain_errno_fcntl(errnum, fildes, command, arg)
            );
        }
        break;

    case F_GETLK:
    case F_SETLK:
    case F_SETLKW:
        {
            static struct flock f_zero;
            struct flock    f;

            if (argc > 2)
            {
                fprintf(stderr, "fcntl: need 2 arguments (not %d)\n", argc);
                exit(EXIT_FAILURE);
            }
            f = f_zero;
            f.l_type = F_RDLCK;
            explain_wrap_and_print
            (
                stdout,
                explain_errno_fcntl(errnum, fildes, command, (long)&f)
            );
        }
        break;

#if defined(F_GETLK64) && F_GETLK != F_GETLK64
    case F_GETLK64:
    case F_SETLK64:
    case F_SETLKW64:
        {
            static struct flock64 f_zero;
            struct flock64  f;

            if (argc != 2)
            {
                fprintf(stderr, "fcntl: need 2 arguments (not %d)", argc);
                exit(EXIT_FAILURE);
            }
            f = f_zero;
            f.l_type = F_RDLCK;
            explain_wrap_and_print
            (
                stdout,
                explain_errno_fcntl(errnum, fildes, command, (long)&f)
            );
        }
        break;
#endif
    }
}


/* vim: set ts=8 sw=4 et : */
