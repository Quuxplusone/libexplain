/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2010-2012 Peter Miller
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

#include <libexplain/ac/assert.h>
#include <libexplain/ac/ctype.h>
#include <libexplain/ac/errno.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/fclose.h>
#include <libexplain/fgets.h>
#include <libexplain/fopen.h>
#include <libexplain/malloc.h>
#include <libexplain/program_name.h>
#include <libexplain/string_list.h>
#include <libexplain/strndup.h>
#include <libexplain/version_print.h>


static explain_string_list_t view_path;
static explain_string_list_t source_files;
static int      error_count;
static char     read_lines_path[1000];


static void
read_lines(const char *filename, explain_string_list_t *content)
{
    FILE            *fp;
    size_t          j;

    fp = 0;
    if (0 == strcmp(filename, "-"))
        fp = stdin;
    else
    {
        for (j = 0; j < view_path.length; ++j)
        {
            if (j >= view_path.length)
            {
                fprintf(stderr, "%s\n", explain_fopen(filename, "r"));
                exit(EXIT_FAILURE);
            }
            snprintf
            (
                read_lines_path,
                sizeof(read_lines_path),
                "%s/%s",
                view_path.string[j],
                filename
            );
            fp = fopen(read_lines_path, "r");
            if (fp)
                break;
            if (errno != ENOENT)
            {
                fprintf(stderr, "%s\n", explain_fopen(read_lines_path, "r"));
                exit(1);
            }
        }
        if (!fp)
        {
            fprintf(stderr, "%s\n", explain_fopen(filename, "r"));
            exit(1);
        }
    }

    for (;;)
    {
        size_t          n;
        char            buffer[1000];

        if (!explain_fgets_or_die(buffer, sizeof(buffer), fp))
            break;
        n = strlen(buffer);
        while (n > 0 && isspace((unsigned char)buffer[n - 1]))
            --n;
        explain_string_list_append_n(content, buffer, n);
    }
    if (fp != stdin)
        explain_fclose_or_die(fp);
}


static char *
extract_include_name(const char *line)
{
    const char    *cp;
    const char    *start;

    cp = line;
    while (isspace((unsigned char)*cp))
        ++cp;
    if (*cp++ != '#')
        return 0;
    while (isspace((unsigned char)*cp))
        ++cp;
    if (0 != memcmp(cp, "include", 7))
        return 0;
    cp += 7;
    while (isspace((unsigned char)*cp))
        ++cp;
    if (*cp++ != '<')
        return 0;
    start = cp;
    while (*cp != '>')
    {
        if (*cp == '\0')
            return 0;
        ++cp;
    }
    if (start == cp)
        return 0;
    return explain_strndup_or_die(start, cp - start);
}


static int
ends_with(const char *haystack, const char *needle)
{
    size_t          haystack_l;
    size_t          needle_l;

    haystack_l = strlen(haystack);
    needle_l = strlen(needle);
    return
        (
            needle_l <= haystack_l
        &&
            0 == memcmp(haystack + haystack_l - needle_l, needle, needle_l)
        );
}


static void
check(const char *filename)
{
    explain_string_list_t isource;
    explain_string_list_t require;
    size_t          j;

    /*
     * Read the file.
     */
    explain_string_list_constructor(&isource);
    read_lines(filename, &isource);

    /*
     * Look for system includes in the file's source.
     */
    explain_string_list_constructor(&require);
    for (j = 0; j < isource.length; ++j)
    {
        const char      *line;
        char            *name;

        line = isource.string[j];
        name = extract_include_name(line);
        if (name)
        {
            if (0 != memcmp(name, "libexplain/", 11))
            {
                char            req[100];

                snprintf(req, sizeof(req), "libexplain/ac/%s", name);
                explain_string_list_append(&require, req);
            }
            free(name);
        }
    }
    explain_string_list_destructor(&isource);

    if (require.length > 0)
    {
        char            *used;

        used = explain_malloc_or_die(require.length);

        /*
         * Examine each project source file.
         * If it includes the file passed in,
         * it must include all of the "require" files.
         */
        for (j = 0; j < source_files.length; ++j)
        {
            const char      *source_file;
            explain_string_list_t source;
            size_t          k;
            int             relevant;

            source_file = source_files.string[j];
            if (!ends_with(source_file, ".c"))
                continue;
            if (0 == strcmp(source_file, "libexplain/libexplain.h"))
                continue;
            memset(used, 0, require.length);
            explain_string_list_constructor(&source);
            read_lines(source_file, &source);

            /*
             * Check each line.
             */
            relevant = 0;
            for (k = 0; k < source.length; ++k)
            {
                const char      *line;
                char            *name;

                line = source.string[k];
                name = extract_include_name(line);
                if (name)
                {
                    size_t          m;

                    if (0 == strcmp(name, filename))
                    {
                        relevant = k + 1;
                        break;
                    }
                    for (m = 0; m < require.length; ++m)
                    {
                        if (0 == strcmp(name, require.string[m]))
                        {
                            used[m] = 1;
                            break;
                        }
                    }
                    free(name);
                }
            }
            explain_string_list_destructor(&source);

            if (relevant)
            {
                /*
                 * Did they use 'em all?
                 */
                for (k = 0; k < require.length; ++k)
                {
                    if (!used[k])
                    {
                        fprintf
                        (
                            stderr,
                            "%s: %d: portability needs #include <%s>\n",
                            read_lines_path,
                            relevant,
                            require.string[k]
                        );
                        ++error_count;
                    }
                }
            }
        }
        free(used);
    }
    explain_string_list_destructor(&require);
}



static void
usage(void)
{
    const char      *prog;

    prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s [ <option>... ] <filename>...\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(EXIT_FAILURE);
}


int
main(int argc, char **argv)
{
    explain_string_list_t libexplain_h;
    explain_string_list_t public_api;
    size_t          j;

    explain_string_list_append(&view_path, ".");
    for (;;)
    {
        int c = getopt(argc, argv, "F:I:V");
        if (c < 0)
            break;
        switch (c)
        {
        case 'F':
            read_lines(optarg, &source_files);
            break;

        case 'I':
            explain_string_list_append(&view_path, optarg);
            break;

        case 'V':
            explain_version_print();
            return EXIT_SUCCESS;

        default:
            usage();
            /* NOTREACHED */
        }
    }
    while (optind < argc)
        explain_string_list_append_unique(&source_files, argv[optind++]);
    if (source_files.length == 0)
    {
        fprintf(stderr, "no source files named\n");
        exit(EXIT_FAILURE);
    }

    /*
     * Find the libexplain/libexplain.h file.
     */
    explain_string_list_constructor(&libexplain_h);
    read_lines("libexplain/libexplain.h", &libexplain_h);

    /*
     * Find all files that constitute the public interface.
     */
    explain_string_list_constructor(&public_api);
    for (j = 0; j < libexplain_h.length; ++j)
    {
        const char      *line;
        char            *name;

        line = libexplain_h.string[j];
        name = extract_include_name(line);
        if (name)
        {
            explain_string_list_append_unique(&public_api, name);
            free(name);
        }
    }
    explain_string_list_destructor(&libexplain_h);

    /*
     * Check each source file in the public API.
     */
    for (j = 0; j < public_api.length; ++j)
        check(public_api.string[j]);
    if (error_count)
    {
        fprintf
        (
            stderr,
            "Found %d fatal error%s\n",
            error_count,
            (error_count == 1 ? "" : "s")
        );
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
