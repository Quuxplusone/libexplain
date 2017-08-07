/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2010, 2012 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/signal.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/socket.h>
#include <libexplain/ac/sys/un.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/accept.h>
#include <libexplain/bind.h>
#include <libexplain/close.h>
#include <libexplain/connect.h>
#include <libexplain/fwrite.h>
#include <libexplain/listen.h>
#include <libexplain/read.h>
#include <libexplain/socket.h>
#include <libexplain/version_print.h>
#include <libexplain/write.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_bind <fildes> <sock_addr> <sock_addr_size>\n");
    fprintf(stderr, "       test_bind -V\n");
    exit(EXIT_FAILURE);
}


static char *pathname;


static void
unlink_pathname(void)
{
    unlink(pathname);
}


static void
timeout(int signum)
{
    (void)signum;
    fprintf(stderr, "no client connect within the timeout\n");
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    int             connect_mode;

    connect_mode = 0;

    for (;;)
    {
        int c = getopt(argc, argv, "cV");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'c':
            connect_mode = 1;
            break;

        case 'V':
            explain_version_print();
            return EXIT_SUCCESS;

        default:
            usage();
        }
    }
    if (optind + 1 != argc)
        usage();
    pathname = argv[optind];

    if (connect_mode)
    {
        int             fildes;
        struct sockaddr_un sadr;
        char            buf[100];
        ssize_t         n;

        fildes = explain_socket_or_die(AF_UNIX, SOCK_STREAM, 0);

        sadr.sun_family = AF_UNIX;
        strncpy(sadr.sun_path, pathname, sizeof(sadr.sun_path));
        explain_connect_or_die(fildes, (struct sockaddr *)&sadr, sizeof(sadr));
        n = explain_read_or_die(fildes, buf, sizeof(buf));
        explain_fwrite_or_die(buf, 1, n, stdout);
    }
    else
    {
        int             fildes;
        struct sockaddr_un sadr;
        struct sockaddr_storage peer_addr_storage;
        struct sockaddr *peer_addr;
        socklen_t       peer_addr_size;
        int             fd2;

        atexit(unlink_pathname);

        fildes = explain_socket_or_die(AF_UNIX, SOCK_STREAM, 0);

        sadr.sun_family = AF_UNIX;
        strncpy(sadr.sun_path, pathname, sizeof(sadr.sun_path));
        explain_bind_or_die(fildes, (struct sockaddr *)&sadr, sizeof(sadr));

        /*
         * We set an alarm so that automated tests that fail do not wait
         * around forever.  It also ensures the socket file is removed.
         */
        signal(SIGALRM, timeout);
        alarm(10);

        explain_listen_or_die(fildes, 1);

        peer_addr = (struct sockaddr *)&peer_addr_storage;
        peer_addr_size = sizeof(peer_addr_storage);
        fd2 = explain_accept_or_die(fildes, peer_addr, &peer_addr_size);
        explain_write_or_die(fd2, "Hello, World!\n", 14);
        explain_close_or_die(fd2);
        explain_close_or_die(fildes);
    }
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
