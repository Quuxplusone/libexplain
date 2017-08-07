/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2011, 2012 Peter Miller
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>
#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/linux/videodev.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/ioctl.h>

#include <libexplain/buffer/long.h>
#include <libexplain/buffer/vbi_format.h>
#include <libexplain/buffer/video_audio.h>
#include <libexplain/buffer/video_buffer.h>
#include <libexplain/buffer/video_capability.h>
#include <libexplain/buffer/video_channel.h>
#include <libexplain/buffer/video_mbuf.h>
#include <libexplain/buffer/video_picture.h>
#include <libexplain/buffer/video_tuner.h>
#include <libexplain/buffer/video_window.h>
#include <libexplain/close.h>
#include <libexplain/ioctl.h>
#include <libexplain/open.h>
#include <libexplain/output.h>
#include <libexplain/program_name.h>
#include <libexplain/wrap_and_print.h>


#ifdef VIDIOCGCAP

static int
ioctl_wary(int fildes, int request, void *data)
{
    int             result;

    result = ioctl(fildes, request, data);
    if (result >= 0)
        return result;
    if (errno != EINVAL)
    {
        explain_output_error_and_die
        (
            "%s",
            explain_ioctl(fildes, request, data)
        );
    }
    return -1;
}

#endif


int
main(int argc, char **argv)
{
    int             fildes;
#ifdef VIDIOCGCAP
    struct video_capability cap;
    struct explain_string_buffer_t sb;
    char            message[1 << 12];
#endif
    const char      *filename;

    explain_program_name_set(argv[0]);
    filename = "/dev/video0";
    switch (argc)
    {
    default:
        {
            const char      *prog;

            prog = explain_program_name_get();
            fprintf(stderr, "Usage: %s <device>\n", prog);
            fprintf(stderr, "       %s -V\n", prog);
            return 1;
        }

    case 2:
        if (argv[1][0] == '-')
            return 0;
        filename = argv[1];
        break;

    case 1:
        break;
    }
    explain_option_hanging_indent_set(4);

    fildes = explain_open_or_die(filename, O_RDWR, 0);

#ifdef VIDIOCGCAP
    /*
     * what are the capabilities?
     */
    explain_ioctl_or_die(fildes, VIDIOCGCAP, &cap);

    explain_string_buffer_init(&sb, message, sizeof(message));
    explain_string_buffer_puts(&sb, "VIDIOCGCAP video_capability = ");
    explain_buffer_video_capability(&sb, &cap);
    explain_wrap_and_print(stdout, message);

    {
        unsigned        j;

        for (j = 0; ; ++j)
        {
            struct video_channel qry;

            memset(&qry, 0, sizeof(qry));
            qry.channel = j;
            if (ioctl_wary(fildes, VIDIOCGCHAN, &qry) < 0)
                break;
            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb, "VIDIOCGCHAN video_channel = ");
            explain_buffer_video_channel(&sb, &qry, 1);
            explain_wrap_and_print(stdout, message);
        }
    }

    {
        struct video_window qry;

        explain_ioctl_or_die(fildes, VIDIOCGWIN, &qry);

        explain_string_buffer_init(&sb, message, sizeof(message));
        explain_string_buffer_puts(&sb, "VIDIOCGWIN video_window = ");
        explain_buffer_video_window(&sb, &qry, 0);
        explain_wrap_and_print(stdout, message);
    }

    {
        struct video_mbuf qry;

        explain_ioctl_or_die(fildes, VIDIOCGMBUF, &qry);

        explain_string_buffer_init(&sb, message, sizeof(message));
        explain_string_buffer_puts(&sb, "VIDIOCGMBUF video_mbuf = ");
        explain_buffer_video_mbuf(&sb, &qry);
        explain_wrap_and_print(stdout, message);
    }

    {
        struct video_picture qry;

        explain_ioctl_or_die(fildes, VIDIOCGPICT, &qry);

        explain_string_buffer_init(&sb, message, sizeof(message));
        explain_string_buffer_puts(&sb, "VIDIOCGPICT video_picture = ");
        explain_buffer_video_picture(&sb, &qry);
        explain_wrap_and_print(stdout, message);
    }

    {
        int             j;

        for (j = 0; ; ++j)
        {
            struct video_tuner qry;

            memset(&qry, 0, sizeof(qry));
            qry.tuner = j;
            if (ioctl_wary(fildes, VIDIOCGTUNER, &qry) < 0)
                break;

            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb, "VIDIOCGTUNER video_tuner = ");
            explain_buffer_video_tuner(&sb, &qry, 1);
            explain_wrap_and_print(stdout, message);
        }
    }

    {
        struct video_buffer qry;

        explain_ioctl_or_die(fildes, VIDIOCGFBUF, &qry);
        explain_string_buffer_init(&sb, message, sizeof(message));
        explain_string_buffer_puts(&sb, "VIDIOCGFBUF video_buffer = ");
        explain_buffer_video_buffer(&sb, &qry);
        explain_wrap_and_print(stdout, message);
    }

    {
        unsigned long   qry;

        if (ioctl_wary(fildes, VIDIOCGFREQ, &qry) >= 0)
        {
            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb, "VIDIOCGFREQ unsigned long = ");
            explain_buffer_ulong(&sb, qry);
            explain_wrap_and_print(stdout, message);
        }
    }

    {
        int             j;

        for (j = 0; ; ++j)
        {
            struct video_audio qry;

            memset(&qry, 0, sizeof(qry));
            qry.audio = j;
            if (ioctl_wary(fildes, VIDIOCGAUDIO, &qry) < 0)
                break;

            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb, "VIDIOCGAUDIO video_audio = ");
            explain_buffer_video_audio(&sb, &qry, 1);
            explain_wrap_and_print(stdout, message);
        }
    }

    {
        struct vbi_format qry;

        if (ioctl_wary(fildes, VIDIOCGVBIFMT, &qry) >= 0)
        {
            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb, "VIDIOCGVBIFMT vbi_format = ");
            explain_buffer_vbi_format(&sb, &qry);
            explain_wrap_and_print(stdout, message);
        }
    }
#endif

    explain_close_or_die(fildes);
    return 0;
}


/* vim: set ts=8 sw=4 et : */
