/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2011, 2013 Peter Miller
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

#ifndef LIBEXPLAIN_BUFFER_V4L2_AUDIOOUT_H
#define LIBEXPLAIN_BUFFER_V4L2_AUDIOOUT_H

#include <libexplain/string_buffer.h>

struct v4l2_audioout; /* forward */

/**
  * The explain_buffer_v4l2_audioout function may be used
  * to print a representation of a v4l2_audioout structure.
  *
  * @param sb
  *     The string buffer to print into.
  * @param data
  *     The v4l2_audioout structure to be printed.
  * @param extra
  *     whether or not to print the returned value fields as well
  */
void explain_buffer_v4l2_audioout(explain_string_buffer_t *sb,
    const struct v4l2_audioout *data, int extra);

void explain_buffer_v4l2_audioout_index(explain_string_buffer_t *sb, int value,
    int fildes);

void explain_buffer_v4l2_audioout_index_ptr(explain_string_buffer_t *sb,
    const int *value, int fildes);

int explain_v4l2_audioout_get_naudioouts(int fildes);

/* vim: set ts=8 sw=4 et : */
#endif /* LIBEXPLAIN_BUFFER_V4L2_AUDIOOUT_H */
