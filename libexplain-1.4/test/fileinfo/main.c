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

#include <libexplain/ac/limits.h> /* for PATH_MAX execpt Solaris */
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/sys/param.h> /* for PATH_MAX on Solaris */
#include <libexplain/ac/unistd.h>

#include <libexplain/fileinfo.h>
#include <libexplain/output.h>
#include <libexplain/program_name.h>
#include <libexplain/strtol.h>
#include <libexplain/version_print.h>


static int
getopt2(int argc, char **argv)
{
    int             c;

    c = getopt(argc, argv, "cef:p:sV");
    if (c < 0 && optind < argc)
    {
        optarg = argv[optind++];
        return 'f';
    }
    return c;
}


static void
usage(void)
{
    const char *prog;
    prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s ( -s | -p<pid> )( -c | -e | -f<fd> )\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(EXIT_FAILURE);
}


static void
must_be_end_of_cmdline(int argc, char **argv)
{
    if (getopt2(argc, argv) > 0)
        usage();
    if (optind < argc)
        usage();
}


int
main(int argc, char **argv)
{
    pid_t pid = 0;
    switch (getopt2(argc, argv))
    {
    case 's':
        /* self */
        pid = getpid();
        break;

    case 'p':
        pid = explain_strtol_or_die(optarg, 0, 0);
        break;

    case 'V':
        must_be_end_of_cmdline(argc, argv);
        explain_version_print();
        return 0;

    default:
        usage();
    }
    switch (getopt2(argc, argv))
    {
    case 'c':
        /* cwd */
        {
            char            path[PATH_MAX + 1];

            must_be_end_of_cmdline(argc, argv);
            if (!explain_fileinfo_pid_cwd(pid, path, sizeof(path)))
            {
                explain_output_error_and_die
                (
                    "unable to determine pid %ld cwd\n",
                    (long)pid
                );
            }
            printf("%s\n", path);
        }
        break;

    case 'e':
        /* exe */
        {
            char            path[PATH_MAX + 1];

            must_be_end_of_cmdline(argc, argv);
            if (!explain_fileinfo_pid_exe(pid, path, sizeof(path)))
            {
                explain_output_error_and_die
                (
                    "unable to determine pid %ld exe\n",
                    (long)pid
                );
            }
            printf("%s\n", path);
        }
        break;

    case 'f':
        {
            int             fildes;
            char            path[PATH_MAX + 1];

            fildes = explain_strtol_or_die(optarg, 0, 0);
            must_be_end_of_cmdline(argc, argv);
            if (!explain_fileinfo_pid_fd_n(pid, fildes, path, sizeof(path)))
            {
                explain_output_error_and_die
                (
                    "unable to determine pid %ld fildes %d\n",
                    (long)pid,
                    fildes
                );
            }
            printf("%s\n", path);
        }
        break;

    default:
        usage();
    }
    return EXIT_SUCCESS;
}

/* vim: set ts=8 sw=4 et : */
