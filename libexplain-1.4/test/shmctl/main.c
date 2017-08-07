/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2011-2013 Peter Miller
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
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/unistd.h>
#include <libexplain/ac/sys/shm.h>

#include <libexplain/buffer/shmctl_command.h>
#include <libexplain/buffer/shmid_ds.h>
#include <libexplain/buffer/shm_info.h>
#include <libexplain/buffer/shminfo.h>
#include <libexplain/output.h>
#include <libexplain/shmctl.h>
#include <libexplain/string_buffer.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/version_print.h>
#include <libexplain/wrap_and_print.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_shmctl <shmid> <command> <data>\n");
    fprintf(stderr, "       test_shmctl -V\n");
    exit(EXIT_FAILURE);
}


static void
print_shm_info(const struct shm_info *data)
{
    char            result[2000];
    explain_string_buffer_t result_sb;

    explain_string_buffer_init(&result_sb, result, sizeof(result));
    explain_buffer_shm_info(&result_sb, data);
    explain_wrap_and_print(stdout, result);
}


static void
print_shminfo(const struct shminfo *data)
{
    char            result[2000];
    explain_string_buffer_t result_sb;

    explain_string_buffer_init(&result_sb, result, sizeof(result));
    explain_buffer_shminfo(&result_sb, data);
    explain_wrap_and_print(stdout, result);
}


static void
print_shmid_ds(int shmid, const struct shmid_ds *data)
{
    char            result[2000];
    explain_string_buffer_t result_sb;

    explain_string_buffer_init(&result_sb, result, sizeof(result));
    if (shmid >= 0)
        explain_string_buffer_printf(&result_sb, "shmid = %d, ", shmid);
    explain_buffer_shmid_ds(&result_sb, data, 1);
    explain_wrap_and_print(stdout, result);
}


static void
list(void)
{
    struct shminfo  shminfo;
    struct shm_info shm_info;
    int             maxid;
    int             id;

    maxid = explain_shmctl_or_die(0, SHM_INFO, (void *)&shm_info);
    print_shm_info(&shm_info);
    explain_shmctl_or_die(0, IPC_INFO, (void *)&shminfo);
    print_shminfo(&shminfo);
    for (id = 0; id < maxid; ++id)
    {
        int             shmid;
        struct shmid_ds shmseg;

        shmid = shmctl(id, SHM_STAT, &shmseg);
        if (shmid < 0)
        {
            if (errno != EINVAL && errno != EIDRM)
            {
                explain_output_error_and_die
                (
                    "%s",
                    explain_shmctl(id, SHM_STAT, &shmseg)
                );
            }
            continue;
        }
        print_shmid_ds(shmid, &shmseg);
    }
}


int
main(int argc, char **argv)
{
    int             shmid;
    int             command;
    struct shmid_ds *data;

    union dummy_t
    {
        int u0;
#ifdef HAVE_SYS_SHM_H
        struct shmid_ds u1;
#endif
#ifdef SHM_INFO
        struct shm_info u2;
#endif
#ifdef IPC_INFO
        struct shminfo u3;
#endif

    };

    union dummy_t   dummy;

    data = (void *)&dummy;
    for (;;)
    {
        int c = getopt(argc, argv, "lV");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'l':
            /* list everything */
            list();
            return 0;

        case 'V':
            explain_version_print();
            return EXIT_SUCCESS;

        default:
            usage();
        }
    }
    switch (argc - optind)
    {
    default:
        usage();

    case 3:
        data = explain_parse_pointer_or_die(argv[optind + 2]);
    case 2:
        command =
            explain_parse_shmctl_command_or_die(argv[optind + 1], "arg 1");
        shmid = explain_parse_int_or_die(argv[optind]);
        break;
    }

    explain_shmctl_or_die(shmid, command, data);

    if (data == (void *)&dummy)
    {
        switch (command)
        {
#ifdef IPC_STAT
        case IPC_STAT:
#endif
#ifdef SHM_STAT
        case SHM_STAT:
#endif
            print_shmid_ds(-1, data);
            break;

#ifdef IPC_INFO
        case IPC_INFO:
            print_shminfo((void *)data);
            break;
#endif

#ifdef SHM_INFO
        case SHM_INFO:
            print_shm_info((void *)data);
            break;
#endif

        default:
            break;
        }
    }

    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
