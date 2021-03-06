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

#include <libexplain/ac/linux/videodev2.h>

#include <libexplain/buffer/int32_t.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/v4l2_chip_ident.h>
#include <libexplain/buffer/v4l2_dbg_chip_ident.h>
#include <libexplain/buffer/v4l2_dbg_match.h>
#include <libexplain/is_efault.h>


#ifdef HAVE_LINUX_VIDEODEV2_H
#ifdef VIDIOC_DBG_G_CHIP_IDENT


void
explain_buffer_v4l2_dbg_chip_ident(explain_string_buffer_t *sb,
    const struct v4l2_dbg_chip_ident *data, int extra)
{
    if (explain_is_efault_pointer(data, sizeof(*data)))
    {
        explain_buffer_pointer(sb, data);
        return;
    }

    explain_string_buffer_puts(sb, "{ match = ");
    explain_buffer_v4l2_dbg_match(sb, &data->match);
    if (extra)
    {
        explain_string_buffer_puts(sb, ", ident = ");
        explain_buffer_v4l2_chip_ident(sb, data->ident);
        explain_string_buffer_puts(sb, ", revision = ");
        explain_buffer_uint32_t(sb, data->revision);
    }
    explain_string_buffer_puts(sb, " }");
}

#endif
#endif

/* vim: set ts=8 sw=4 et : */
