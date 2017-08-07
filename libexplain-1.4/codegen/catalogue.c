/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010, 2012 Peter Miller
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

#include <libexplain/ac/assert.h>
#include <libexplain/ac/ctype.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/fclose.h>
#include <libexplain/fopen.h>
#include <libexplain/getc.h>
#include <libexplain/malloc.h>
#include <libexplain/strdup.h>

#include <codegen/aegis.h>
#include <codegen/boolean.h>
#include <codegen/catalogue.h>
#include <codegen/elastic_buffer.h>
#include <codegen/header.h>
#include <codegen/wrapper.h>


static const char *const search_path[] =
{
    "bl",
    "bl/bl",
    "bl/bl/bl",
    0
};


static int
exists(const char *filename)
{
    return (access(filename, F_OK) >= 0);
}


static void
find(const char *filename, char *buffer, size_t buffer_size)
{
    size_t          j;

    if (exists(filename))
    {
        snprintf(buffer, buffer_size, "%s", filename);
        return;
    }
    for (j = 0; ; ++j)
    {
        const char      *dir;

        dir = search_path[j];
        if (!dir)
            break;
        snprintf(buffer, buffer_size, "%s/%s", dir, filename);
        if (exists(buffer))
            return;
    }
    snprintf(buffer, buffer_size, "%s", filename);
}


static catalogue_t *
catalogue_new(void)
{
    catalogue_t *result = explain_malloc_or_die(sizeof(catalogue_t));
    result->data = 0;
    result->data_length = 0;
    result->data_maximum = 0;
    return result;
}


static int
read_one_line(FILE *fp, char *line, size_t line_size)
{
    char *line_end = line + line_size - 1;
    char *cp = line;
    for (;;)
    {
        int c = explain_getc_or_die(fp);
        if (c == EOF)
        {
            done:
            *cp = '\0';
            return (cp != line);
        }
        if (c == '\n')
        {
            c = explain_getc_or_die(fp);
            if (c == EOF)
                goto done;
            if (c != ' ' && c != '\t')
            {
                ungetc(c, fp);
                goto done;
            }
            for (;;)
            {
                c = explain_getc_or_die(fp);
                if (c == EOF)
                    break;
                if (c != ' ' && c != '\t')
                {
                    ungetc(c, fp);
                    break;
                }
            }
            c = ' ';
        }
        if (cp < line_end)
            *cp++ = c;
    }
}


static void
catalogue_load(catalogue_t *cp, const char *filename)
{
    FILE            *fp;
    char            buffer[4000];

    find(filename, buffer, sizeof(buffer));
    fp = explain_fopen_or_die(buffer, "r");
    while (read_one_line(fp, buffer, sizeof(buffer)))
    {
        char            *name;
        char            *value;

        if (buffer[0] == '#')
            continue;
        name = buffer;
        value = strchr(buffer, ':');
        if (value)
        {
            *value++ = '\0';
            while (*value && isspace((unsigned char)*value))
                ++value;
            catalogue_set(cp, name, value);
        }
    }
    explain_fclose_or_die(fp);
}


catalogue_t *
catalogue_open(const char *fn)
{
    catalogue_t *result = catalogue_new();
    catalogue_load(result, fn);
    catalogue_set(result, "Filename", fn);
    return result;
}


catalogue_t *
catalogue_creat(const char *fn)
{
    catalogue_t     *result;

    aegis_new_file(fn);
    result = catalogue_new();
    catalogue_set(result, "Filename", fn);
    return result;
}


void
catalogue_close(catalogue_t *cp)
{
    size_t          j;

    for (j = 0; j < cp->data_length; ++j)
    {
        catalogue_data_t *cdp;

        cdp = cp->data[j];
        free(cdp->name);
        free(cdp->value);
        free(cdp);
    }
    if (cp->data)
        free(cp->data);
    free(cp);
}


const char *
catalogue_get(catalogue_t *cp, const char *name)
{
    catalogue_data_t *cdp;
    size_t          j;

    for (j = 0; j < cp->data_length; ++j)
    {
        cdp = cp->data[j];
        if (0 == strcasecmp(cdp->name, name))
            return cdp->value;
    }
    return 0;
}


int
catalogue_get_bool(catalogue_t *cp, const char *name, int dflt)
{
    catalogue_data_t *cdp;
    size_t          j;

    for (j = 0; j < cp->data_length; ++j)
    {
        cdp = cp->data[j];
        if (0 == strcasecmp(cdp->name, name))
            return string_to_boolean(cdp->value);
    }
    catalogue_set_bool(cp, name, dflt);
    return dflt;
}


int
catalogue_get_int(catalogue_t *cp, const char *name, int dflt)
{
    size_t          j;

    for (j = 0; j < cp->data_length; ++j)
    {
        catalogue_data_t *cdp;

        cdp = cp->data[j];
        if (0 == strcasecmp(cdp->name, name))
        {
            const char      *s;
            char            *ep;
            long            n;

            s = cdp->value;
            ep = 0;
            n = strtol(s, &ep, 0);
            if (ep != s && !*ep)
                return n;
            break;
        }
    }
    catalogue_set_int(cp, name, dflt);
    return dflt;
}


void
catalogue_set(catalogue_t *cp, const char *name, const char *value)
{
    catalogue_data_t *cdp;
    size_t          j;

    for (j = 0; j < cp->data_length; ++j)
    {
        cdp = cp->data[j];
        if (0 == strcasecmp(cdp->name, name))
        {
            free(cdp->value);
            cdp->value = explain_strdup_or_die(value);
            return;
        }
    }

    if (cp->data_length >= cp->data_maximum)
    {
        size_t new_maximum = cp->data_maximum * 2 + 16;
        catalogue_data_t **new_data =
            explain_malloc_or_die(new_maximum * sizeof(catalogue_data_t *));
        for (j = 0; j < cp->data_length; ++j)
            new_data[j] = cp->data[j];
        if (cp->data)
            free(cp->data);
        cp->data = new_data;
        cp->data_maximum = new_maximum;
    }

    cdp = explain_malloc_or_die(sizeof(catalogue_data_t));
    cdp->name = explain_strdup_or_die(name);
    cdp->value = explain_strdup_or_die(value);
    cp->data[cp->data_length++] = cdp;
}


void
catalogue_set_int(catalogue_t *cp, const char *name, int value)
{
    char            buffer[30];

    snprintf(buffer, sizeof(buffer), "%d", value);
    catalogue_set(cp, name, buffer);
}


void
catalogue_set_bool(catalogue_t *cp, const char *name, int value)
{
    catalogue_set(cp, name, boolean_to_string(value));
}


void
catalogue_save(catalogue_t *cp)
{
    catalogue_data_t *cdp;
    size_t          j;
    const char      *filename;
    FILE            *fp;
    elastic_buffer_t sb;

    elastic_buffer_constructor(&sb);
    filename = catalogue_get(cp, "Filename");
    assert(filename);
    fp = explain_fopen_or_die(filename, "w");
    gpl_header(fp, "#");
    for (j = 0; j < cp->data_length; ++j)
    {
        cdp = cp->data[j];
        if (0 != strcasecmp(cdp->name, "Filename"))
        {
            elastic_buffer_rewind(&sb);
            elastic_buffer_printf(&sb, "%s: %s", cdp->name, cdp->value);
            wrapper_hang(fp, "", elastic_buffer_get(&sb));
        }
    }
    fprintf(fp, "# vi" "m: set ts=8 sw=4 et :\n");
    explain_fclose_or_die(fp);
    elastic_buffer_destructor(&sb);
}


/* vim: set ts=8 sw=4 et : */
