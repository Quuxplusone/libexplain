/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010 Peter Miller
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

#include <libexplain/ac/ctype.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/unistd.h>

#include <codegen/header.h>
#include <codegen/wrapper.h>
#include <codegen/get_user_name.h>


void
gpl_header(FILE *fp, const char *prefix)
{
    char            prefix1[20];
    char            prefix2[20];

    /*
     * Massage the prefix to be sane.
     */
    {
        size_t          len;

        len = strlen(prefix);
        while (len > 0 && isspace((unsigned char)prefix[len - 1]))
            --len;
        snprintf(prefix1, sizeof(prefix1), "%.*s", (int)len, prefix);
        snprintf(prefix2, sizeof(prefix2), "%.*s ", (int)len, prefix);
    }

    if (strchr(prefix, '*'))
        fprintf(fp, "/*\n");
    else
        fprintf(fp, "%s\n", prefix1);

    wrapper(fp, prefix2,
        "libexplain - Explain errno values returned by libc functions");

    /*
     * Print the copyright notice.
     */
    {
        char            buffer[1000];

        snprintf
        (
            buffer,
            sizeof(buffer),
            "Copyright (C) %d %s\n",
            get_year(),
            get_user_name()
        );
        wrapper_hang(fp, prefix2, buffer);
    }

    fprintf(fp, "%s\n", prefix1);
    wrapper(fp, prefix2,
        "This program is free software; you can redistribute it and/or modify "
        "it under the terms of the GNU General Public License as "
        "published by the Free Software Foundation; either version 3 of the "
        "License, or (at your option) any later version."
    );
    fprintf(fp, "%s\n", prefix1);
    wrapper(fp, prefix2,
        "This program is distributed in the hope that it will be useful, "
        "but WITHOUT ANY WARRANTY; without even the implied warranty of "
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU "
        "Lesser General Public License for more details."
    );
    fprintf(fp, "%s\n", prefix1);
    wrapper(fp, prefix2,
        "You should have received a copy of the GNU General "
        "Public License along with this program. If not, see "
        "<http://www.gnu.org/licenses/>."
    );

    if (strchr(prefix, '*'))
        fprintf(fp, " */\n");
    else
        fprintf(fp, "%s\n", prefix1);
}


void
lgpl_header(FILE *fp, const char *prefix)
{
    char            prefix1[20];
    char            prefix2[20];

    /*
     * Massage the prefix to be sane.
     */
    {
        size_t          len;

        len = strlen(prefix);
        while (len > 0 && isspace((unsigned char)prefix[len - 1]))
            --len;
        snprintf(prefix1, sizeof(prefix1), "%.*s", (int)len, prefix);
        snprintf(prefix2, sizeof(prefix2), "%.*s ", (int)len, prefix);
    }

    if (strchr(prefix, '*'))
        fprintf(fp, "/*\n");
    else
        fprintf(fp, "%s\n", prefix1);

    wrapper(fp, prefix2,
        "libexplain - Explain errno values returned by libc functions");

    /*
     * Print the copyright notice.
     */
    {
        char            buffer[1000];

        snprintf
        (
            buffer,
            sizeof(buffer),
            "Copyright (C) %d %s\n",
            get_year(),
            get_user_name()
        );
        wrapper(fp, prefix2, buffer);
    }

    fprintf(fp, "%s\n", prefix1);
    wrapper(fp, prefix2,
        "This program is free software; you can redistribute it and/or modify "
        "it under the terms of the GNU Lesser General Public License as "
        "published by the Free Software Foundation; either version 3 of the "
        "License, or (at your option) any later version."
    );
    fprintf(fp, "%s\n", prefix1);
    wrapper(fp, prefix2,
        "This program is distributed in the hope that it will be useful, "
        "but WITHOUT ANY WARRANTY; without even the implied warranty of "
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU "
        "Lesser General Public License for more details."
    );
    fprintf(fp, "%s\n", prefix1);
    wrapper(fp, prefix2,
        "You should have received a copy of the GNU Lesser General "
        "Public License along with this program. If not, see "
        "<http://www.gnu.org/licenses/>."
    );

    if (strchr(prefix, '*'))
        fprintf(fp, " */\n");
    else
        fprintf(fp, "%s\n", prefix1);
}
