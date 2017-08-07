/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2011, 2013 Peter Miller
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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/shm.h>

#include <libexplain/buffer/shmctl_command.h>
#include <libexplain/buffer/shmid_ds.h>
#include <libexplain/buffer/shm_info.h>
#include <libexplain/buffer/shminfo.h>
#include <libexplain/shmctl.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/shmctl.h>


void
explain_syscall_shmctl(int errnum, int argc, char **argv)
{
    int             shmid;
    int             command;
    struct shmid_ds *data;
    union data_t
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
    union data_t    dummy;

    memset(&dummy, 0, sizeof(dummy));
    data = &dummy.u1;
    switch (argc)
    {
    case 3:
        data = explain_parse_pointer_or_die(argv[2]);
        /* Fall through... */

    case 2:
        command =
            explain_parse_shmctl_command_or_die(argv[1], "arg 2");
        shmid = explain_parse_int_or_die(argv[0]);
        break;

    default:
        fprintf(stderr, "shmctl: requires 3 arguments, not %d\n", argc);
        exit(EXIT_FAILURE);
    }

    explain_wrap_and_print(stdout, explain_errno_shmctl(errnum, shmid, command,
        data));
}


/* vim: set ts=8 sw=4 et : */
