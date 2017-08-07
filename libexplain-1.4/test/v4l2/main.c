/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2011, 2012, 2014 Peter Miller
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
#include <libexplain/ac/linux/videodev2.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/ioctl.h>

#include <libexplain/buffer/v4l2_audio.h>
#include <libexplain/buffer/v4l2_sliced_vbi_cap.h>
#include <libexplain/buffer/v4l2_audioout.h>
#include <libexplain/buffer/v4l2_capability.h>
#include <libexplain/buffer/v4l2_control.h>
#include <libexplain/buffer/v4l2_dv_enum_preset.h>
#include <libexplain/buffer/v4l2_dv_preset.h>
#include <libexplain/buffer/v4l2_dv_timings.h>
#include <libexplain/buffer/v4l2_enc_idx.h>
#include <libexplain/buffer/v4l2_fmtdesc.h>
#include <libexplain/buffer/v4l2_format.h>
#include <libexplain/buffer/v4l2_frequency.h>
#include <libexplain/buffer/v4l2_frmivalenum.h>
#include <libexplain/buffer/v4l2_frmsizeenum.h>
#include <libexplain/buffer/v4l2_input.h>
#include <libexplain/buffer/v4l2_jpegcompression.h>
#include <libexplain/buffer/v4l2_modulator.h>
#include <libexplain/buffer/v4l2_output.h>
#include <libexplain/buffer/v4l2_priority.h>
#include <libexplain/buffer/v4l2_queryctrl.h>
#include <libexplain/buffer/v4l2_standard.h>
#include <libexplain/buffer/v4l2_std_id.h>
#include <libexplain/buffer/v4l2_streamparm.h>
#include <libexplain/buffer/v4l2_tuner.h>
#include <libexplain/close.h>
#include <libexplain/ioctl.h>
#include <libexplain/open.h>
#include <libexplain/output.h>
#include <libexplain/program_name.h>
#include <libexplain/wrap_and_print.h>


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


int
main(int argc, char **argv)
{
    int             fildes;
#ifdef HAVE_LINUX_VIDEODEV2_H
    struct v4l2_capability cap;
#endif
    struct explain_string_buffer_t sb;
    const char      *filename;
    char            message[1 << 12];

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

#ifdef HAVE_LINUX_VIDEODEV2_H
    /*
     * what are the capabilities?
     */
    {
        explain_ioctl_or_die(fildes, VIDIOC_QUERYCAP, &cap);

        explain_string_buffer_init(&sb, message, sizeof(message));
        explain_string_buffer_puts(&sb, "VIDIOC_QUERYCAP v4l2_capability = ");
        explain_buffer_v4l2_capability(&sb, &cap);
        explain_wrap_and_print(stdout, message);
    }

    /*
     * Video capture features.
     */
    if (cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)
    {
        int             j;

        for (j = 0; ; ++j)
        {
            struct v4l2_fmtdesc fmtdesc;

            memset(&fmtdesc, 0, sizeof(fmtdesc));
            fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            fmtdesc.index = j;
            if (ioctl_wary(fildes, VIDIOC_ENUM_FMT, &fmtdesc) < 0)
                break;
            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb, "VIDIOC_ENUM_FMT v4l2_fmtdesc = ");
            explain_buffer_v4l2_fmtdesc(&sb, &fmtdesc, 1);
            explain_wrap_and_print(stdout, message);
        }

        j = 0;
        for (;;)
        {
            struct v4l2_queryctrl qry;
            struct v4l2_control ctrl;

            memset(&qry, 0, sizeof(qry));
            qry.id = j | V4L2_CTRL_FLAG_NEXT_CTRL;
            qry.type = V4L2_CTRL_TYPE_INTEGER;

            if (ioctl_wary(fildes, VIDIOC_QUERYCTRL, &qry) < 0)
                break;

            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb,
                "VIDIOC_QUERYCTRL v4l2_queryctrl = ");
            explain_buffer_v4l2_queryctrl(&sb, &qry, 1, fildes);
            explain_wrap_and_print(stdout, message);

            memset(&ctrl, 0, sizeof(ctrl));
            ctrl.id = qry.id;
            explain_ioctl_or_die(fildes, VIDIOC_G_CTRL, &ctrl);

            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb, "VIDIOC_G_CTRL v4l2_control = ");
            explain_buffer_v4l2_control(&sb, &ctrl, 1, fildes);
            explain_wrap_and_print(stdout, message);

            j = qry.id;
        }

        {
            enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            struct v4l2_streamparm parm;

            memset(&parm, 0, sizeof(parm));
            parm.type = type;
            explain_ioctl_or_die(fildes, VIDIOC_G_PARM, &parm);

            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb, "VIDIOC_G_PARM v4l2_streamparm = ");
            explain_buffer_v4l2_streamparm(&sb, &parm, 1);
            explain_wrap_and_print(stdout, message);
        }
    }

    /*
     * Tuners (receivers)
     */
    {
        int             ntuners;
        int             j;

        /*
         * Most video drivers that support VIDIOC_G_TUNER have the same
         * bug: they fail to check the index for out-of-range values.
         */
        ntuners = explain_v4l2_tuner_get_n(fildes);
        for (j = 0; j < ntuners; ++j)
        {
            struct v4l2_tuner qry;

            memset(&qry, 0, sizeof(qry));
            qry.index = j;
            explain_ioctl_or_die(fildes, VIDIOC_G_TUNER, &qry);

            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb, "VIDIOC_G_TUNER v4l2_tuner = ");
            explain_buffer_v4l2_tuner(&sb, &qry, 1);
            explain_wrap_and_print(stdout, message);
        }
    }
    {
        int             ntuners;
        int             j;

        /*
         * Most video drivers that support VIDIOC_G_FREQUENCY have the same
         * bug: they fail to check the qry.tuner for out-of-range values.
         */
        ntuners = explain_v4l2_frequency_get_n(fildes);
        for (j = 0; j < ntuners; ++j)
        {
            struct v4l2_frequency qry;

            memset(&qry, 0, sizeof(qry));
            qry.tuner = j;
            explain_ioctl_or_die(fildes, VIDIOC_G_FREQUENCY, &qry);

            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb,
                "VIDIOC_G_FREQUENCY v4l2_frequency = ");
            explain_buffer_v4l2_frequency(&sb, &qry, 1);
            explain_wrap_and_print(stdout, message);
        }
    }

    /*
     * Modulators (transmitters)
     */
    {
        int             nmodulators;
        int             j;

        /*
         * Most video drivers that support VIDIOC_G_MODULATOR have the same
         * bug: they fail to check the index for out-of-range values.
         */
        nmodulators = explain_v4l2_modulator_get_n(fildes);
        for (j = 0; j < nmodulators; ++j)
        {
            struct v4l2_modulator qry;

            memset(&qry, 0, sizeof(qry));
            qry.index = j;
            explain_ioctl_or_die(fildes, VIDIOC_G_MODULATOR, &qry);

            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb,
                "VIDIOC_G_MODULATOR v4l2_modulator = ");
            explain_buffer_v4l2_modulator(&sb, &qry, 1);
            explain_wrap_and_print(stdout, message);
        }
    }

    {
        struct v4l2_jpegcompression qry;

        memset(&qry, 0, sizeof(qry));
        if (ioctl_wary(fildes, VIDIOC_G_JPEGCOMP, &qry) >= 0)
        {
            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb,
            "VIDIOC_G_JPEGCOMP v4l2_jpegcompression = ");
            explain_buffer_v4l2_jpegcompression(&sb, &qry);
            explain_wrap_and_print(stdout, message);
        }
    }

    {
        enum v4l2_priority qry;

        if (ioctl_wary(fildes, VIDIOC_G_PRIORITY, &qry) >= 0)
        {
            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb,
            "VIDIOC_G_PRIORITY v4l2_priority = ");
            explain_buffer_v4l2_priority(&sb, qry);
            explain_wrap_and_print(stdout, message);
        }
    }

    /*
     * Video input frame sizes.
     */
    {
        struct v4l2_format fmt;

        memset(&fmt, 0, sizeof(fmt));
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (ioctl_wary(fildes, VIDIOC_G_FMT, &fmt) >= 0)
        {
            int             j;

            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb,
                "VIDIOC_G_FMT v4l2_format = ");
            explain_buffer_v4l2_format(&sb, &fmt, 1);
            explain_wrap_and_print(stdout, message);

            for (j = 0; ; ++j)
            {
                struct v4l2_frmsizeenum qry;

                memset(&qry, 0, sizeof(qry));
                qry.index = j;
                qry.pixel_format = fmt.fmt.pix.pixelformat;
                if (ioctl_wary(fildes, VIDIOC_ENUM_FRAMESIZES, &qry) < 0)
                    break;

                explain_string_buffer_init(&sb, message, sizeof(message));
                explain_string_buffer_puts(&sb,
                    "VIDIOC_ENUM_FRAMESIZES v4l2_frmsizeenum = ");
                explain_buffer_v4l2_frmsizeenum(&sb, &qry, 1);
                explain_wrap_and_print(stdout, message);
            }

            for (j = 0; ; ++j)
            {
                struct v4l2_frmivalenum qry;

                memset(&qry, 0, sizeof(qry));
                qry.index = j;
                qry.pixel_format = fmt.fmt.pix.pixelformat;
                if (ioctl_wary(fildes, VIDIOC_ENUM_FRAMEINTERVALS, &qry) < 0)
                    break;

                explain_string_buffer_init(&sb, message, sizeof(message));
                explain_string_buffer_puts(&sb,
                    "VIDIOC_ENUM_FRAMEINTERVALS v4l2_frmivalenum = ");
                explain_buffer_v4l2_frmivalenum(&sb, &qry, 1);
                explain_wrap_and_print(stdout, message);
            }
        }
    }

    /*
     * Video input features.
     */
    {
        int             j;

        for (j = 0; ; ++j)
        {
            struct v4l2_input in;

            memset(&in, 0, sizeof(in));
            in.index = j;
            if (ioctl_wary(fildes, VIDIOC_ENUMINPUT, &in) < 0)
                break;

            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb,
                "VIDIOC_ENUMINPUT v4l2_input = ");
            explain_buffer_v4l2_input(&sb, &in, 1);
            explain_wrap_and_print(stdout, message);
        }
    }
    {
        int             curin;

        if (ioctl_wary(fildes, VIDIOC_G_INPUT, &curin) >= 0)
        {
            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb,
                "VIDIOC_G_INPUT int = ");
            explain_buffer_v4l2_input_index(&sb, curin, fildes);
            explain_wrap_and_print(stdout, message);
        }
    }

    /*
     * Audio input features.
     */
    {
        int             j;

        for (j = 0; ; ++j)
        {
            struct v4l2_audio in;

            memset(&in, 0, sizeof(in));
            in.index = j;
            if (ioctl_wary(fildes, VIDIOC_ENUMAUDIO, &in) < 0)
                break;

            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb,
                "VIDIOC_ENUMAUDIO v4l2_audio = ");
            explain_buffer_v4l2_audio(&sb, &in, 1);
            explain_wrap_and_print(stdout, message);
        }
    }
    {
        int             current;

        if (ioctl_wary(fildes, VIDIOC_G_AUDIO, &current) >= 0)
        {
            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb,
                "VIDIOC_G_AUDIO int = ");
            explain_buffer_v4l2_audio_index(&sb, current, fildes);
            explain_wrap_and_print(stdout, message);
        }
    }

    /*
     * Video output features.
     */
    {
        int             j;

        for (j = 0; ; ++j)
        {
            struct v4l2_output in;

            memset(&in, 0, sizeof(in));
            in.index = j;
            if (ioctl_wary(fildes, VIDIOC_ENUMOUTPUT, &in) < 0)
                break;

            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb,
                "VIDIOC_ENUMOUTPUT v4l2_output = ");
            explain_buffer_v4l2_output(&sb, &in, 1);
            explain_wrap_and_print(stdout, message);
        }
    }
    {
        int             idx;

        if (ioctl_wary(fildes, VIDIOC_G_OUTPUT, &idx) >= 0)
        {
            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb,
                "VIDIOC_G_OUTPUT int = ");
            explain_buffer_v4l2_output_index(&sb, idx, fildes);
            explain_wrap_and_print(stdout, message);
        }
    }

    if (cap.capabilities & V4L2_CAP_VIDEO_OUTPUT)
    {
        enum v4l2_buf_type type;
        int             j;

        type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
        for (j = 0; ; ++j)
        {
            struct v4l2_standard std;

            memset(&std, 0, sizeof(std));
            std.index = j;
            if (ioctl_wary(fildes, VIDIOC_ENUMSTD, &std) < 0)
                break;

            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb, "VIDIOC_ENUMSTD v4l2_standard = ");
            explain_buffer_v4l2_standard(&sb, &std, 1);
            explain_wrap_and_print(stdout, message);
        }

        {
            v4l2_std_id     std;

            if (ioctl_wary(fildes, VIDIOC_G_STD, &std) >= 0)
            {
                explain_string_buffer_init(&sb, message, sizeof(message));
                explain_string_buffer_puts(&sb, "VIDIOC_G_STD v4l2_std_id = ");
                explain_buffer_v4l2_std_id(&sb, std);
                explain_wrap_and_print(stdout, message);
            }
        }

        {
            struct v4l2_streamparm parm;

            memset(&parm, 0, sizeof(parm));
            parm.type = type;
            explain_ioctl_or_die(fildes, VIDIOC_G_PARM, &parm);

            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb, "VIDIOC_G_PARM v4l2_streamparm = ");
            explain_buffer_v4l2_streamparm(&sb, &parm, 1);
            explain_wrap_and_print(stdout, message);
        }
    }

    /*
     * Audio output features.
     */
    {
        int             j;

        for (j = 0; ; ++j)
        {
            struct v4l2_audioout qry;

            memset(&qry, 0, sizeof(qry));
            qry.index = j;
            if (ioctl_wary(fildes, VIDIOC_ENUMAUDOUT, &qry) < 0)
                break;

            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb,
                "VIDIOC_ENUMAUDOUT v4l2_audioout = ");
            explain_buffer_v4l2_audioout(&sb, &qry, 1);
            explain_wrap_and_print(stdout, message);
        }
    }
    {
        int             idx;

        if (ioctl_wary(fildes, VIDIOC_G_AUDOUT, &idx) >= 0)
        {
            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb,
                "VIDIOC_G_AUDOUT int = ");
            explain_buffer_v4l2_audioout_index(&sb, idx, fildes);
            explain_wrap_and_print(stdout, message);
        }
    }

#ifdef VIDIOC_ENUM_DV_PRESETS
    {
        int             j;

        for (j = 0; ; ++j)
        {
            struct v4l2_dv_enum_preset qry;

            memset(&qry, 0, sizeof(qry));
            qry.index = j;
            if (ioctl_wary(fildes, VIDIOC_ENUM_DV_PRESETS, &qry) < 0)
                break;

            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb,
                "VIDIOC_ENUM_DV_PRESETS v4l2_dv_enum_preset = ");
            explain_buffer_v4l2_dv_enum_preset(&sb, &qry, 1);
            explain_wrap_and_print(stdout, message);
        }
    }
#endif
#if defined(VIDIOC_QUERY_DV_PRESET) && defined(VIDIOC_G_DV_PRESET)
    {
        struct v4l2_dv_preset qry;

        memset(&qry, 0, sizeof(qry));
        if (ioctl_wary(fildes, VIDIOC_QUERY_DV_PRESET, &qry) >= 0)
        {
            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb,
                "VIDIOC_QUERY_DV_PRESET v4l2_dv_preset = ");
            explain_buffer_v4l2_dv_preset(&sb, &qry);
            explain_wrap_and_print(stdout, message);
        }

        memset(&qry, 0, sizeof(qry));
        if (ioctl_wary(fildes, VIDIOC_G_DV_PRESET, &qry) >= 0)
        {
            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb,
                "VIDIOC_G_DV_PRESET v4l2_dv_preset = ");
            explain_buffer_v4l2_dv_preset(&sb, &qry);
            explain_wrap_and_print(stdout, message);
        }
    }
#endif
#ifdef VIDIOC_G_DV_TIMINGS
    {
        struct v4l2_dv_timings qry;

        memset(&qry, 0, sizeof(qry));
        if (ioctl_wary(fildes, VIDIOC_G_DV_TIMINGS, &qry) >= 0)
        {
            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb,
                "VIDIOC_G_DV_TIMINGS v4l2_dv_timings = ");
            explain_buffer_v4l2_dv_timings(&sb, &qry);
            explain_wrap_and_print(stdout, message);
        }
    }
#endif /* VIDIOC_G_DV_TIMINGS */
    {
        struct v4l2_sliced_vbi_cap qry;

        memset(&qry, 0, sizeof(qry));
        if (ioctl_wary(fildes, VIDIOC_G_SLICED_VBI_CAP, &qry) >= 0)
        {
            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb,
                "VIDIOC_G_SLICED_VBI_CAP v4l2_sliced_vbi_cap = ");
            explain_buffer_v4l2_sliced_vbi_cap(&sb, &qry, 1);
            explain_wrap_and_print(stdout, message);
        }
    }
    {
        struct v4l2_enc_idx qry;

        memset(&qry, 0, sizeof(qry));
        if (ioctl_wary(fildes, VIDIOC_G_ENC_INDEX, &qry) >= 0)
        {
            explain_string_buffer_init(&sb, message, sizeof(message));
            explain_string_buffer_puts(&sb,
                "VIDIOC_G_ENC_INDEX v4l2_enc_idx = ");
            explain_buffer_v4l2_enc_idx(&sb, &qry);
            explain_wrap_and_print(stdout, message);
        }
    }
#endif

    explain_close_or_die(fildes);
    return 0;
}


/* vim: set ts=8 sw=4 et : */
