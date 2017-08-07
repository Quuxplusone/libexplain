/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2013 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/open_flags.h>
#include <libexplain/buffer/permission_mode.h>
#include <libexplain/open.h>
#include <libexplain/openat.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_openat <fildes> <pathname> <flags> <mode>\n");
    fprintf(stderr, "       test_openat -V\n");
    exit(EXIT_FAILURE);
}


static int
is_a_number(const char *text)
{
    char *ep = 0;
    if (strtol(text, &ep, 0))
    {
    }
    return (ep != text && *ep == '\0');
}


int
main(int argc, char **argv)
{
    int             fildes;
    const char      *pathname;
    int             flags;
    mode_t          mode;

    for (;;)
    {
        int c = getopt(argc, argv, "V");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'V':
            explain_version_print();
            return EXIT_SUCCESS;

        default:
            usage();
        }
    }
    fildes = 0;
    pathname = 0;
    flags = 0;
    mode = 06666;
    switch (argc - optind)
    {
    case 4:
        mode = explain_permission_mode_parse_or_die(argv[optind + 3], "arg 3");
        /* Fall through... */

    case 3:
        flags = explain_parse_open_flags_or_die(argv[optind + 2], "arg 2");
        /* Fall through... */

    case 2:
        pathname = argv[optind + 1];
        /* Fall through... */

    case 1:
        if (is_a_number(argv[optind]))
            fildes = explain_parse_int_or_die(argv[optind]);
        else if (0 == strcmp(argv[optind], "AT_FDCWD"))
            fildes = AT_FDCWD;
        else
            fildes = explain_open_or_die(argv[optind], O_RDONLY, 0);
        break;

    default:
        usage();
    }

    explain_openat_or_die(fildes, pathname, flags, mode);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
