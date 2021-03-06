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
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/ioctl.h>

#include <libexplain/buffer/einval.h>
#include <libexplain/buffer/is_the_null_pointer.h>
#include <libexplain/buffer/long.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/buffer/video_tuner.h>
#include <libexplain/iocontrol/generic.h>
#include <libexplain/iocontrol/vidiocsfreq.h>
#include <libexplain/is_efault.h>

#ifdef VIDIOCSFREQ


static void
print_data(const explain_iocontrol_t *p, explain_string_buffer_t *sb,
    int errnum, int fildes, int request, const void *data)
{
    struct video_tuner qry;
    const unsigned long *arg;

    (void)p;
    (void)errnum;
    (void)request;
    arg = data;
    if (explain_is_efault_pointer(arg, sizeof(*arg)))
    {
        explain_buffer_pointer(sb, arg);
    }
    else
    {
        explain_buffer_ulong(sb, *arg);
        memset(&qry, 0, sizeof(qry));
        qry.tuner = *arg;
        if (ioctl(fildes, VIDIOCGTUNER, &qry) >= 0 && qry.name[0])
        {
            explain_string_buffer_putc(sb, ' ');
            explain_string_buffer_puts_quoted_n(sb, qry.name, sizeof(qry.name));
        }
    }
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

        {
            const unsigned long *arg;
            int             ntuners;

            arg = data;
            if (explain_is_efault_pointer(arg, sizeof(*arg)))
                goto generic;

            ntuners = explain_video_tuner_get_n(fildes);
            if (ntuners == 0)
            {
                errnum = ENOTTY;
                goto generic;
            }
            if (ntuners > 0 && *arg >= (unsigned)ntuners)
            {
                explain_buffer_einval_out_of_range
                (
                    sb,
                    "data",
                    0,
                    ntuners - 1
                );
                return;
            }
        }

        explain_buffer_einval_vague(sb, "data");
        break;

    default:
        generic:
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


const explain_iocontrol_t explain_iocontrol_vidiocsfreq =
{
    "VIDIOCSFREQ", /* name */
    VIDIOCSFREQ, /* value */
    0, /* disambiguate */
    0, /* print_name */
    print_data,
    print_explanation,
    0, /* print_data_returned */
    sizeof(unsigned long), /* data_size */
    "unsigned long *", /* data_type */
    0, /* flags */
    __FILE__,
    __LINE__,
};

#else /* ndef VIDIOCSFREQ */

const explain_iocontrol_t explain_iocontrol_vidiocsfreq =
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

#endif /* VIDIOCSFREQ */

/* vim: set ts=8 sw=4 et : */
