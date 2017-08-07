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

#include <ioctl-scan/report.h>


void
report(const char *iname, const char *name, const char *value)
{
    char            buf[80];

    snprintf(buf, sizeof(buf), "%.*s: ", (int)(sizeof(buf) - 3), iname);
    printf("%-24s", buf);
    snprintf(buf, sizeof(buf), "%.*s: ", (int)(sizeof(buf) - 3), name);
    printf("%-8s", buf);
    printf("%s\n", value);
}


void
report_int(const char *request, const char *name, int value, const char *units)
{
    char            buf[80];

    if (units && *units)
        snprintf(buf, sizeof(buf), "%5d %s", value, units);
    else
        snprintf(buf, sizeof(buf), "%5d", value);
    report(request, name, buf);
}


void
report_long(const char *iname, const char *name, long value, const char *units)
{
    char            buf[80];

    if (units && *units)
        snprintf(buf, sizeof(buf), "%5ld %s", value, units);
    else
        snprintf(buf, sizeof(buf), "%5ld", value);
    report(iname, name, buf);
}


void
report_ushort(const char *iname, const char *name, unsigned short value,
    const char *units)
{
    char            buf[80];

    if (units && *units)
        snprintf(buf, sizeof(buf), "%5hu %s", value, units);
    else
        snprintf(buf, sizeof(buf), "%5hu", value);
    report(iname, name, buf);
}


void
report_uint64(const char *iname, const char *name, uint64_t value,
    const char *units)
{
    char            buf[80];

    if (units && *units)
    {
        snprintf
        (
            buf,
            sizeof(buf),
            "%5llu %s",
            (unsigned long long)value,
            units
        );
    }
    else
        snprintf(buf, sizeof(buf), "%5llu", (unsigned long long)value);
    report(iname, name, buf);
}


void
report_bool(const char *iname, const char *name, int value)
{
    char            buf[20];

    switch (value)
    {
    case 0:
        report(iname, name, "false");
        break;

    case 1:
        report(iname, name, "true");
        break;

    default:
        snprintf(buf, sizeof(buf), "%5d", value);
        report(iname, name, buf);
        break;
    }
}
