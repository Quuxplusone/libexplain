/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2011, 2013 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>
#include <libexplain/ac/linux/videodev.h>
#include <libexplain/ac/sys/ioctl.h>

#include <libexplain/buffer/boolean.h>
#include <libexplain/buffer/is_the_null_pointer.h>
#include <libexplain/iocontrol/generic.h>
#include <libexplain/iocontrol/vidioccapture.h>

#ifdef VIDIOCCAPTURE


static void
print_data(const explain_iocontrol_t *p, explain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    (void)p;
    (void)errnum;
    (void)fildes;
    (void)request;
    explain_buffer_boolean_ptr(sb, data);
}


static void
print_explanation(const explain_iocontrol_t *p, explain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    switch (errnum)
    {
    case EINVAL:
        if (!data)
        {
            explain_buffer_is_the_null_pointer(sb, "data");
            return;
        }

        errnum = ENOTTY;
        /* Fall through... */

    default:
        explain_iocontrol_generic_print_explanation
        (
            p,
            sb,
            errnum,
            fildes,
            request,
            data
        );
        break;
    }
}


const explain_iocontrol_t explain_iocontrol_vidioccapture =
{
    "VIDIOCCAPTURE", /* name */
    VIDIOCCAPTURE, /* value */
    0, /* disambiguate */
    0, /* print_name */
    print_data,
    print_explanation,
    0, /* print_data_returned */
    sizeof(int), /* data_size */
    "int *", /* data_type */
    0, /* flags */
    __FILE__,
    __LINE__,
};

#else /* ndef VIDIOCCAPTURE */

const explain_iocontrol_t explain_iocontrol_vidioccapture =
{
    0, /* name */
    0, /* value */
    0, /* disambiguate */
    0, /* print_name */
    0, /* print_data */
    0, /* print_explanation */
    0, /* print_data_returned */
    0, /* data_size */
    0, /* data_type */
    0, /* flags */
    __FILE__,
    __LINE__,
};

#endif /* VIDIOCCAPTURE */

/* vim: set ts=8 sw=4 et : */
