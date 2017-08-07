/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009, 2013 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBEXPLAIN_BUFFER_FD_SET_H
#define LIBEXPLAIN_BUFFER_FD_SET_H

#include <libexplain/ac/sys/select.h>

#include <libexplain/string_buffer.h>

/**
  * The explain_buffer_fd_set function may be used to
  * print a representation of a file descriptor set.
  *
  * @param sb
  *    The string buffer to print into.
  * @param nfds
  *    Maximum number of file descriptors in set.
  * @param fds
  *    Pointer to the file descriptor set of interest.
  */
void explain_buffer_fd_set(explain_string_buffer_t *sb, int nfds,
    const fd_set *fds);

#endif /* LIBEXPLAIN_BUFFER_FD_SET_H */
/* vim: set ts=8 sw=4 et : */
