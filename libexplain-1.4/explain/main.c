/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2011 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/getopt.h>

#include <libexplain/errno_info.h>
#include <libexplain/errno_info/print.h>
#include <libexplain/exit.h>
#include <libexplain/freopen.h>
#include <libexplain/iocontrol.h>
#include <libexplain/string_buffer.h>
#include <libexplain/version_print.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: explain -e <errno> <function> [ <args> ... ]\n");
    fprintf(stderr, "       explain -m <message> <function> [ <args> ... ]\n");
    fprintf(stderr, "       explain -V\n");
    exit(EXIT_FAILURE);
}


#ifdef HAVE_GETOPT_LONG

static const struct option options[] =
{
    { "errno", 1, 0, 'e' },
    { "explain-exit-status", 0, 0, 'E' },
    { "message", 1, 0, 'm' },
    { "print-errno-info", 0, 0, 'P' },
    { "statistics", 0, 0, 's' },
    { "version", 0, 0, 'V' },
    { "check-ioctl-conflicts", 0, 0, 'Z' },
    { 0, 0, 0, 0 }
};

#endif


static int exit_status;


static int
figure_out_error(const char *text)
{
    long            n;
    char            *ep;
    const explain_errno_info_t *eip;
    explain_string_buffer_t sb;
    char            message[200];

    explain_string_buffer_init(&sb, message, sizeof(message));

    /* is it a number? */
    n = strtol(text, &ep, 0);
    if (ep != text && !*ep)
        return n;

    /* is it a symbol? */
    eip = explain_errno_info_by_name(text);
    if (eip)
        return eip->error_number;

    /* is it an exact text? */
    eip = explain_errno_info_by_text(text);
    if (eip)
        return eip->error_number;

    /* is it a fuzzy symbol? */
    eip = explain_errno_info_by_name_fuzzy(text);
    if (eip)
    {
        explain_string_buffer_puts(&sb, "the error ");
        explain_string_buffer_puts_quoted(&sb, text);
        explain_string_buffer_puts
        (
            &sb,
            " doesn't match any known symbol, closest is the "
        );
        explain_string_buffer_puts_quoted(&sb, eip->name);
        explain_string_buffer_puts(&sb, " symbol");
        explain_wrap_and_print(stderr, message);
        exit_status = EXIT_FAILURE;
        exit(EXIT_FAILURE);
        /* NOTREACHED */
        return eip->error_number;
    }

    /* is it a fuzzy text? */
    eip = explain_errno_info_by_text_fuzzy(text);
    if (eip)
    {
        explain_string_buffer_puts(&sb, "the error ");
        explain_string_buffer_puts_quoted(&sb, text);
        explain_string_buffer_puts
        (
            &sb,
            " doesn't match any known symbol, closest is the "
        );
        explain_string_buffer_puts_quoted
        (
            &sb,
            explain_internal_strerror(eip->error_number)
        );
        explain_string_buffer_puts(&sb, " message");
        explain_wrap_and_print(stderr, message);
        exit_status = EXIT_FAILURE;
        exit(EXIT_FAILURE);
        /* NOTREACHED */
        return eip->error_number;
    }

    explain_string_buffer_puts(&sb, "the error ");
    explain_string_buffer_puts_quoted(&sb, text);
    explain_string_buffer_puts
    (
        &sb,
        " doesn't look like a number, or an error symbol, or a "
        "strerror() string, aborting"
    );
    explain_wrap_and_print(stderr, message);
    exit(EXIT_FAILURE);
    /* NOTREACHED */
    return 0;
}


static void
print_statistics(void)
{
    int             syscall_total;
    int             ioctl_total;
    int             ioctl_active;

    syscall_statistics(&syscall_total);
    printf("Coverage includes %d system calls ", syscall_total);

    explain_iocontrol_statistics(&ioctl_total, &ioctl_active);
    printf("and %d ioctl requests.\n", ioctl_total);
    if (ioctl_active < ioctl_total)
        printf(".\\\" only %d ioctls relevant to this system\n", ioctl_active);
}


int
main(int argc, char **argv)
{
    func_t          func;
    int             err;

    exit_status = EXIT_SUCCESS;
    err = -1;
    for (;;)
    {
#ifdef HAVE_GETOPT_LONG
        int c = getopt_long(argc, argv, "Ee:o:PpsVZ", options, 0);
#else
        int c = getopt(argc, argv, "Ee:o:PpsVZ");
#endif
        if (c == EOF)
            break;
        switch (c)
        {
        case 'E':
            explain_exit_on_exit();
            break;

        case 'e':
            err = figure_out_error(optarg);
            break;

        case 'o':
            explain_freopen_or_die(optarg, "w", stdout);
            break;

        case 'p':
            explain_errno_info_print(0);
            return 0;

        case 'P':
            explain_errno_info_print(1);
            return 0;

        case 's':
            print_statistics();
            return 0;

        case 'V':
            explain_version_print();
            return 0;

        case 'Z':
            explain_iocontrol_check_conflicts();
            return 0;

        default:
            usage();
            /* NOTREACHED */
        }
    }
    if (err < 0)
    {
        fprintf(stderr, "please specify an error number (-e)\n");
        exit(EXIT_FAILURE);
    }
    if (optind >= argc)
        usage();
    func = find_function(argv[optind]);
    ++optind;
    func(err, argc - optind, argv + optind);
    return exit_status;
}
