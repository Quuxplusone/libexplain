/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010-2012 Peter Miller
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

#include <libexplain/ac/errno.h>
#include <libexplain/ac/stdarg.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/fopen.h>
#include <libexplain/buffer/pathname.h>
#include <libexplain/version_print.h>
#include <libexplain/vfprintf.h>
#include <libexplain/wrap_and_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_vfprintf\n");
    fprintf(stderr, "       test_vfprintf -V\n");
    exit(EXIT_FAILURE);
}


static void
test(int errnum, FILE *fp, const char *fmt, ...)
{
    va_list         ap;

    printf("\n");
    va_start(ap, fmt);
    explain_wrap_and_print(stdout, explain_errno_vfprintf(errnum, fp, fmt, ap));
    va_end(ap);
}


int
main(int argc, char **argv)
{
    FILE            *fp;

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
    if (optind != argc)
        usage();

    fp = explain_fopen_or_die("/dev/null", "w");
    test(EINVAL, fp, NULL);
    test(EINVAL, fp, "...%l%...");
    test(EINVAL, fp, "...%_...");
    test(EINVAL, fp, "...%2$ 'I#0lzjqLh_...");
    test(EINVAL, fp, "%1$d%3$d", 1, 2, 3);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
