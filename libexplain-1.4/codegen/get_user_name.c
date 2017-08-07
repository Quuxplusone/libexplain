/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/pwd.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/time.h>
#include <libexplain/ac/unistd.h>

#include <codegen/get_user_name.h>


const char *
get_user_name(void)
{
    static char      user_name[100];

    if (!user_name[0])
    {
        struct passwd   *pw;
        char            *cp;

        pw = getpwuid(getuid());
        snprintf
        (
            user_name,
            sizeof(user_name),
            "%s",
            (pw ? pw->pw_gecos : "you")
        );
        cp = strchr(user_name, ',');
        if (cp)
            *cp = '\0';
    }
    return user_name;
}


int
get_year(void)
{
    time_t          now;
    struct tm       *tmp;

    time(&now);
    tmp = localtime(&now);
    return (1900 + tmp->tm_year);
}
