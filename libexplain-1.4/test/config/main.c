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

#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/linux/videodev.h>
#include <libexplain/ac/linux/videodev2.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/unistd.h>
#include <libexplain/ac/sys/eventfd.h> /* for EFD_NONBLOCK */

#include <libexplain/fstrcmp.h>
#include <libexplain/output.h>
#include <libexplain/program_name.h>
#include <libexplain/sizeof.h>
#include <libexplain/version_print.h>


typedef struct table_t table_t;
struct table_t
{
    const char *name;
    int value;
};

static const table_t table[] =
{
#include <test_config/configured.h>

    {
        "__linux__",
#ifdef __linux__
        1
#else
        0
#endif
    },
    {
        "__unix__",
#ifdef __unix__
        1
#else
        0
#endif
    },
    {
        "__GNUC__",
#ifdef __GNUC__
        1
#else
        0
#endif
    },
    {
        "__FreeBSD__",
#ifdef __FreeBSD__
        1
#else
        0
#endif
    },

    /*
     * These are so we can be selective in tests.  We don't want too
     * many, because the various ioctl includes do not play nive with
     * each other.
     */
    {
        "VIDIOC_DQEVENT",
#ifdef VIDIOC_DQEVENT
        1
#else
        0
#endif
    },
    {
        "VIDIOC_SUBSCRIBE_EVENT",
#ifdef VIDIOC_SUBSCRIBE_EVENT
        1
#else
        0
#endif
    },
    {
        "VIDIOC_UNSUBSCRIBE_EVENT",
#ifdef VIDIOC_UNSUBSCRIBE_EVENT
        1
#else
        0
#endif
    },
    {
        "VIDIOC_S_HW_FREQ_SEEK",
#ifdef VIDIOC_S_HW_FREQ_SEEK
        1
#else
        0
#endif
    },
    {
        "VIDIOC_DBG_G_CHIP_IDENT",
#ifdef VIDIOC_DBG_G_CHIP_IDENT
        1
#else
        0
#endif
    },
    {
        "VIDIOC_QUERY_DV_PRESET",
#ifdef VIDIOC_QUERY_DV_PRESET
        1
#else
        0
#endif
    },
    {
        "VIDIOC_G_DV_PRESET",
#ifdef VIDIOC_G_DV_PRESET
        1
#else
        0
#endif
    },
    {
        "VIDIOC_ENUM_DV_PRESETS",
#ifdef VIDIOC_ENUM_DV_PRESETS
        1
#else
        0
#endif
    },
    {
        "VIDIOC_G_DV_TIMINGS",
#ifdef VIDIOC_G_DV_TIMINGS
        1
#else
        0
#endif
    },
    {
        "VIDIOCGCAP",
#ifdef VIDIOCGCAP
        1
#else
        0
#endif
    },
    {
        "O_NOFOLLOW",
#if defined(O_NOFOLLOW) && (O_NOFOLLOW != 0)
        1
#else
        0
#endif
    },
    {
        "O_DIRECTORY",
#if defined(O_DIRECTORY) && (O_DIRECTORY != 0)
        1
#else
        0
#endif
    },
    {
        "O_CLOEXEC",
#if defined(O_CLOEXEC) && (O_CLOEXEC != 0)
        1
#else
        0
#endif
    },
    {
        "O_DIRECT",
#if defined(O_DIRECT) && (O_DIRECT != 0)
        1
#else
        0
#endif
    },
    {
        "O_NOATIME",
#if defined(O_NOATIME) && (O_NOATIME != 0)
        1
#else
        0
#endif
    },
    {
        "EFD_NONBLOCK",
#ifdef EFD_NONBLOCK
        1
#else
        0
#endif
    },
};


static void
usage(void)
{
    const char *prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s <cfg-name>\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(EXIT_SUCCESS);
}


int
main(int argc, char **argv)
{
    const char      *name;
    const table_t   *tp;
    const table_t   *best;
    double          best_weight;

    explain_option_hanging_indent_set(4);
    for (;;)
    {
        int c = getopt(argc, argv, "lV");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'l':
            for (tp = table; tp < ENDOF(table); ++tp)
            {
                printf("#%s %s\n", (tp->value ? "define" : "undef"), tp->name);
            }
            return EXIT_SUCCESS;

        case 'V':
            explain_version_print();
            return EXIT_SUCCESS;

        default:
            usage();
        }
    }
    if (optind + 1 != argc)
        usage();
    name = argv[optind];
    for (tp = table; tp < ENDOF(table); ++tp)
    {
        if (0 == strcmp(tp->name, name))
        {
            if (tp->value)
                return EXIT_SUCCESS;
            explain_output_error_and_die
            (
                "This system does not %s, therefore this test is declared to "
                    "pass by default.",
                name
            );
            return EXIT_FAILURE;
        }
    }
    if (0 == strcmp(name, "not-root"))
    {
        if (getuid() == 0 || geteuid() == 0)
        {
            explain_output_error_and_die
            (
                "This test only applies when not executed by root, "
                "therefore this test is declared to pass by default."
            );
        }
        return EXIT_SUCCESS;
    }

    /*
     * See of we can find a close match, in case it was a typo.
     */
    best = 0;
    best_weight = 0.6;
    for (tp = table; tp < ENDOF(table); ++tp)
    {
        double          w;

        w = explain_fstrcasecmp(tp->name, name);
        if (w > best_weight)
        {
            best = tp;
            best_weight = w;
        }
    }
    if (best)
    {
        explain_output_error_and_die
        (
            "Configuration item \"%s\" unknown, did you mean \"%s\" instead?  "
                "Proceeding with the test.",
            name,
            best->name
        );
    }

    /*
     * Nothing similar, they will have to work it out for themselves.
     */
    explain_output_error_and_die
    (
        "Configuration item \"%s\" unknown, proceeding with test.",
        name
    );
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
