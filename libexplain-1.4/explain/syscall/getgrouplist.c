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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/pwd.h>
#include <libexplain/ac/sys/types.h>

#include <libexplain/getgrouplist.h>
#include <libexplain/malloc.h>
#include <libexplain/output.h>
#include <libexplain/string_to_thing.h>
#include <libexplain/wrap_and_print.h>

#include <explain/syscall/getgrouplist.h>


void
explain_syscall_getgrouplist(int errnum, int argc, char **argv)
{
    const char      *user;
    gid_t           group;
    int             group_set = 0;
    gid_t           *groups;
    int             ngroups;

    user = 0;
    group = 0;
    groups = 0;
    ngroups = 0;
    /*
     * <user> [[ <group> [ <groups> ] ] <ngroups> ]
     */
    switch (argc)
    {
    case 4:
        ngroups = explain_parse_int_or_die(argv[3]);
        groups = explain_parse_pointer_or_die(argv[3]);
        group = explain_parse_gid_t_or_die(argv[1]);
        group_set = 1;
        user = argv[0];
        break;

    case 3:
        ngroups = explain_parse_int_or_die(argv[3]);
        group = explain_parse_gid_t_or_die(argv[1]);
        group_set = 1;
        user = argv[0];
        break;

    case 2:
        ngroups = explain_parse_int_or_die(argv[3]);
        user = argv[0];
        break;

    case 1:
        user = argv[0];
        break;

    default:
        explain_output_error_and_die
        (
            "getgrouplist: requires 4 arguments, not %d\n",
            argc
        );
    }
    if (!groups)
    {
        size_t groups_size = (ngroups < 1 ? 1 : ngroups) * sizeof(*groups);
        groups = explain_malloc_or_die(groups_size);
    }
    if (!group_set)
    {
        struct passwd *pw = getpwnam(user);
        if (!pw)
            explain_output_error_and_die("user \"%s\" unknown", user);
        group = pw->pw_gid;
    }
    explain_wrap_and_print
    (
        stdout,
        explain_errno_getgrouplist(errnum, user, group, groups, &ngroups)
    );
}

/* vim: set ts=8 sw=4 et : */
