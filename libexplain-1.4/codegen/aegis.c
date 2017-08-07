/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
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
#include <libexplain/system.h>

#include <codegen/aegis.h>



void
aegis_new_file(const char *filename)
{
    char            command[1000];

    fprintf(stderr, "creating %s\n", filename);
    snprintf
    (
        command,
        sizeof(command),
        "( aegis --new-file '%s' || aegis --copy-file '%s' ) > /dev/null 2>&1",
        filename,
        filename
    );
    explain_system_or_die(command);
}


void
aegis_copy_file(const char *filename)
{
    char            command[1000];

    fprintf(stderr, "modify %s\n", filename);
    snprintf
    (
        command,
        sizeof(command),
        "aegis --copy-file '%s' > /dev/null 2>&1",
        filename
    );
    explain_system_or_die(command);
}
