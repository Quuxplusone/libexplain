/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2012 Peter Miller
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
#include <libexplain/ac/pwd.h>
#include <libexplain/ac/unistd.h>


int
main(int argc, char **argv)
{
    int             u;
    struct passwd   *pw;

    (void)argc;
    (void)argv;
    u = geteuid();
    pw = getpwuid(u);
    if (pw)
        printf("%s\n", pw->pw_name);
    else
        printf("%d\n", u);
    return 0;

}


/* vim: set ts=8 sw=4 et : */
