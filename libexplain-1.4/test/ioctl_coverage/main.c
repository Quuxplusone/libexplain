/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2011, 2012 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/ctype.h>
#include <libexplain/ac/dirent.h>
#include <libexplain/ac/errno.h>
#include <libexplain/ac/limits.h> /* for PATH_MAX on Solaris */
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/param.h> /* for PATH_MAX except Solaris */
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/time.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/closedir.h>
#include <libexplain/fclose.h>
#include <libexplain/fflush.h>
#include <libexplain/fgets.h>
#include <libexplain/fopen.h>
#include <libexplain/iocontrol.h>
#include <libexplain/lstat.h>
#include <libexplain/malloc.h>
#include <libexplain/opendir.h>
#include <libexplain/output.h>
#include <libexplain/program_name.h>
#include <libexplain/readdir.h>
#include <libexplain/strdup.h>
#include <libexplain/system.h>
#include <libexplain/version_print.h>


static int      supported;
static int      unsupported;
static int      verbose;
static int      generate;


static void
mkdir_p(const char *path)
{
    const char      *slash;
    size_t          nbytes;
    char            path2[PATH_MAX + 1];

    if (mkdir(path, 0755) >= 0)
        return;
    if (errno != ENOENT)
        return;
    slash = strrchr(path, '/');
    if (!slash || slash == path)
        return;

    nbytes = slash - path;
    if (nbytes >= sizeof(path2))
        return;
    memcpy(path2, path, nbytes);
    path2[nbytes] = '\0';
    mkdir_p(path2);

    mkdir(path, 0755);
}


static int
year(void)
{
    time_t          now;
    struct tm       *tmp;

    time(&now);
    tmp = localtime(&now);
    return (1900 + tmp->tm_year);
}


static int
is_a_number(const char *text)
{
    char *ep = 0;
    long n = strtol(text, &ep, 0);
    (void)n;
    return (ep != text && !*ep);
}


static void
generate_code(const char *request_name, const char *request_definition,
    const char *include_file_name)
{
    char            request_name_lc[100];
    int             flags;
    char            third[100];
    char            *third2;
    char            c_fn[PATH_MAX + 1];
    FILE            *c_fp;
    char            h_fn[PATH_MAX + 1];
    FILE            *h_fp;
    int             print_expl;

    enum
    {
        flag_read = 1,
        flag_write = 2,
        flag_numeric = 4
    };

    if (0 == memcmp(include_file_name, "/usr/include/", 13))
        include_file_name += 13;

    /* make a lower-case version of the ioctl name */
    {
        const char *ip = request_name;
        char *op = request_name_lc;
        char *end = op + sizeof(request_name_lc) - 1;
        while (*ip && op < end)
        {
            unsigned char c = *ip++;
            if (isupper(c))
                c = tolower(c);
            *op++ = c;
        }
        *op = '\0';
    }
    print_expl = 0;

    /* extract the third argument of the ioctl definition */
    flags = 0;
    third[0] = '\0';
    {
        const char      *cp;
        const char      *third_begin;
        size_t          third_size;

        cp = request_definition + 3;
        if (*cp == 'W')
        {
            flags |= flag_write;
            ++cp;
        }
        if (*cp == 'R')
        {
            flags |= flag_read;
            ++cp;
        }
        if (*cp == 'N')
        {
            flags |= flag_numeric;
            ++cp;
        }

        /* skip first argument */
        while (*cp)
        {
            ++cp;
            if (cp[-1] == ',')
                break;
        }
        while (isspace((unsigned char)*cp))
            ++cp;

        /* skip second argument */
        while (*cp)
        {
            ++cp;
            if (cp[-1] == ',')
                break;
        }
        while (isspace((unsigned char)*cp))
            ++cp;

        /* grab third argument (even if there isn't one) */
        third_begin = cp;
        while (*cp && *cp != ')')
            ++cp;
        third_size = cp - third_begin;
        if (third_size > sizeof(third) - 1)
            third_size = sizeof(third) - 1;
        memcpy(third, third_begin, third_size);
        third[third_size] = '\0';

        if
        (
            !third[0]
        &&
            (flags & (flag_numeric))
        &&
            (flags & (flag_read | flag_write))
        )
            explain_strendcpy(third, "int", third + sizeof(third));
        else if (is_a_number(third))
            explain_strendcpy(third, "int", third + sizeof(third));

        third2 = third;
        if (0 == memcmp(third, "struct ", 7))
            third2 += 7;
    }

    mkdir_p("libexplain/iocontrol");
    snprintf(c_fn, sizeof(c_fn), "libexplain/iocontrol/%s.c", request_name_lc);
    if (verbose)
        fprintf(stderr, "Writing \"%s\"...\n", c_fn);
    c_fp = explain_fopen_or_die(c_fn, "w");

    fprintf
    (
        c_fp,
"/*\n"
" * libexplain - a library of system-call-specific strerror replacements\n"
" * Copyright (C) %d Peter Miller\n",
        year()
    );
    fprintf
    (
        c_fp,
" * Written by Peter Miller <pmiller@opensource.org.au>\n"
" *\n"
" * This program is free software; you can redistribute it and/or modify it\n"
" * under the terms of the GNU Lesser General Public License as published by\n"
" * the Free Software Foundation; either version 3 of the License, or (at\n"
" * your option) any later version.\n"
" *\n"
" * This program is distributed in the hope that it will be useful,\n"
" * but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser\n"
" * General Public License for more details.\n"
" *\n"
" * You should have received a copy of the GNU Lesser General Public License\n"
" * along with this program. If not, see <http://www.gnu.org/licenses/>.\n"
" */\n"
"\n"
    );
    if (print_expl)
        fprintf(c_fp, "#include <libexplain/ac/errno.h>\n");
    fprintf
    (
        c_fp,
"#include <libexplain/ac/sys/ioctl.h>\n"
"#include <libexplain/ac/%s>\n",
        include_file_name
    );
    if (print_expl)
        fprintf(c_fp, "\n#include <libexplain/buffer/is_the_null_pointer.h>\n");
    if (third[0])
        fprintf(c_fp, "#include <libexplain/buffer/%s.h>\n", third2);
    if (print_expl)
        fprintf(c_fp, "#include <libexplain/iocontrol/generic.h>\n");
    fprintf(c_fp, "#include <libexplain/iocontrol/%s.h>\n", request_name_lc);
    fprintf
    (
        c_fp,
        "\n"
        "#ifdef %s\n",
        request_name
    );
    if ((flags & flag_write) && 0 != strcmp(third, "int"))
    {
        fprintf
        (
            c_fp,
"\n"
"\n"
"static void\n"
"print_data(const explain_iocontrol_t *p, explain_string_buffer_t *sb,\n"
"    int errnum, int fildes, int request, const void *data)\n"
"{\n"
"    (void)p;\n"
"    (void)errnum;\n"
"    (void)fildes;\n"
"    (void)request;\n"
"    explain_buffer_%s(sb, data);\n"
"}\n",
            third2
        );
    }
    if (print_expl)
    {
        fprintf
        (
            c_fp,
"\n"
"\n"
"static void\n"
"print_explanation(const explain_iocontrol_t *p, explain_string_buffer_t *sb,\n"
"    int errnum, int fildes, int request, const void *data)\n"
"{\n"
"    switch (errnum)\n"
"    {\n"
"    case EFAULT:\n"
"        if (!data)\n"
"        {\n"
"            explain_buffer_is_the_null_pointer(sb, \"data\");\n"
"            return;\n"
"        }\n"
    );
    if (flags & flag_write)
    {
        fprintf
        (
            c_fp,
            "\n"
            "        if (...fill.me.in...)\n"
            "        {\n"
            "            ...print.the.explanation...\n"
            "            return;\n"
            "        }\n"
            "\n"
            "        /* Nothing obvious... */\n"
        );
    }
    fprintf
    (
        c_fp,
"        goto generic;\n"
"\n"
"    case EINVAL:\n"
"        if (!data)\n"
"        {\n"
"            explain_buffer_is_the_null_pointer(sb, \"data\");\n"
"            return;\n"
"        }\n"
        );
    if (flags & flag_write)
    {
        fprintf
        (
            c_fp,
            "\n"
            "        if (...fill.me.in...)\n"
            "        {\n"
            "            ...print.the.explanation...\n"
            "            return;\n"
            "        }\n"
            "\n"
            "        /* Nothing obvious... */\n"
        );
    }
    fprintf
    (
        c_fp,
        "        goto generic;\n"
        "\n"
        "    default:\n"
        "        generic:\n"
        "        explain_iocontrol_generic_print_explanation\n"
        "        (\n"
        "            p,\n"
        "            sb,\n"
        "            errnum,\n"
        "            fildes,\n"
        "            request,\n"
        "            data\n"
        "        );\n"
        "        break;\n"
        "    }\n"
        "}\n"
        );
    }
    if ((flags & flag_read) && 0 != strcmp(third, "int"))
    {
        fprintf
        (
            c_fp,
        "\n"
        "\n"
        "static void\n"
        "print_data_returned(const explain_iocontrol_t *p, "
        "explain_string_buffer_t *sb,\n"
        "    int errnum, int fildes, int request, const void *data)\n"
        "{\n"
        "    (void)p;\n"
        "    (void)errnum;\n"
        "    (void)fildes;\n"
        "    (void)request;\n"
        "    explain_buffer_%s(sb, data);\n"
        "}\n",
            third2
        );
    }
    fprintf
    (
        c_fp,
        "\n"
        "\n"
        "const explain_iocontrol_t explain_iocontrol_%s =\n",
        request_name_lc
    );
    fprintf
    (
        c_fp,
        "{\n"
        "    \"%s\", /* name */\n",
        request_name
    );
    fprintf
    (
        c_fp,
        "    %s, /* value */\n",
        request_name
    );
    fprintf
    (
        c_fp,
        "    0, /* disambiguate */\n"
        "    0, /* print_name */\n"
    );
    if (flags & flag_write)
    {
        if (0 == strcmp(third, "int"))
        {
            fprintf
            (
                c_fp,
         "    explain_iocontrol_generic_print_data_int_star, /* print_data */\n"
            );
        }
        else
            fprintf(c_fp, "    print_data,\n");
    }
    else
    {
        if (flags & flag_read)
        {
            fprintf
            (
                c_fp,
          "    explain_iocontrol_generic_print_data_pointer, /* print_data */\n"
            );
        }
        else
            fprintf(c_fp, "    0, /* print_data */\n");
    }
    if (print_expl)
        fprintf(c_fp, "    print_explanation,\n");
    else
        fprintf(c_fp, "    0, /* print_explanation */\n");
    if (flags & flag_read)
    {
        if (0 == strcmp(third, "int"))
        {
            fprintf
            (
                c_fp,
"    explain_iocontrol_generic_print_data_int_star, /* print_data_returned */\n"
            );
        }
        else
            fprintf(c_fp, "    print_data_returned,\n");
    }
    else
        fprintf(c_fp, "    0, /* print_data_returned */\n");
    if (flags & (flag_read | flag_write))
    {
        fprintf(c_fp, "    sizeof(%s), /* data_size */\n", third);
        fprintf(c_fp, "    \"%s *\", /* data_type */\n", third);
    }
    else
    {
        fprintf(c_fp, "    NOT_A_POINTER, /* data_size */\n");
        fprintf(c_fp, "    \"intptr_t\", /* data_type */\n");
    }
    fprintf
    (
        c_fp,
        "    0, /* flags */\n"
        "    __FILE__,\n"
        "    __LINE__,\n"
        "};\n"
        "\n"
        "#else /* ndef %s */\n",
        request_name
    );
    fprintf
    (
        c_fp,
        "\n"
        "const explain_iocontrol_t explain_iocontrol_%s =\n"
        "{\n"
        "    0, /* name */\n"
        "    0, /* value */\n"
        "    0, /* disambiguate */\n"
        "    0, /* print_name */\n"
        "    0, /* print_data */\n"
        "    0, /* print_explanation */\n"
        "    0, /* print_data_returned */\n"
        "    0, /* data_size */\n"
        "    0, /* data_type */\n"
        "    0, /* flags */\n"
        "    __FILE__,\n"
        "    __LINE__,\n"
        "};\n",
        request_name_lc
    );
    fprintf
    (
        c_fp,
        "\n"
        "#endif /* %s */\n"
        "\n"
        "/* v" "im" ": set ts=8 sw=4 et : */",
        request_name
    );
    explain_fclose_or_die(c_fp);
    c_fp = 0;

    /*
     * And the corresponding include file.
     */
    snprintf(h_fn, sizeof(h_fn), "libexplain/iocontrol/%s.h", request_name_lc);
    if (verbose)
        fprintf(stderr, "Writing \"%s\"...\n", h_fn);
    h_fp = explain_fopen_or_die(h_fn, "w");

    fprintf
    (
        h_fp,
"/*\n"
" * libexplain - a library of system-call-specific strerror replacements\n"
" * Copyright (C) %d Peter Miller\n",
        year()
    );
    fprintf
    (
        h_fp,
" * Written by Peter Miller <pmiller@opensource.org.au>\n"
" *\n"
" * This program is free software; you can redistribute it and/or modify it\n"
" * under the terms of the GNU Lesser General Public License as published by\n"
" * the Free Software Foundation; either version 3 of the License, or (at\n"
" * your option) any later version.\n"
" *\n"
" * This program is distributed in the hope that it will be useful,\n"
" * but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
" * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser\n"
" * General Public License for more details.\n"
" *\n"
" * You should have received a copy of the GNU Lesser General Public License\n"
" * along with this program. If not, see <http://www.gnu.org/licenses/>.\n"
" */\n"
"\n"
"#ifndef LIBEXPLAIN_IOCONTROL_%s_H\n",
        request_name
    );
    fprintf
    (
        h_fp,
        "#define LIBEXPLAIN_IOCONTROL_%s_H\n",
        request_name
    );
    fprintf
    (
        h_fp,
        "\n"
        "#include <libexplain/iocontrol.h>\n"
        "\n"
        "/**\n"
        "  * The explain_iocontrol_%s global variable is used\n",
        request_name_lc
    );
    fprintf
    (
        h_fp,
        "  * to store information about the %s ioctl(2) request.\n",
        request_name
    );
    fprintf
    (
        h_fp,
        "  *\n"
        "  * @note\n"
        "  *     This information is not kept in a single table for all\n"
        "  *     values, like every other set of constants, because (a) some\n"
        "  *     values are ambiguous, and (b) the include files have bugs\n"
        "  *     making it impossible to include all of them in the same\n"
        "  *     compilation unit.\n"
        "  */\n"
        "extern const explain_iocontrol_t explain_iocontrol_%s;\n",
        request_name_lc
    );
    fprintf
    (
        h_fp,
        "\n"
        "/* v" "im" ": set ts=8 sw=4 et : */\n"
        "#endif /* LIBEXPLAIN_IOCONTROL_%s_H */\n",
        request_name
    );
    explain_fclose_or_die(h_fp);
    h_fp = 0;

    /*
     * tell Aegis about the new files
     */
    if (generate >= 2)
    {
        char            command[1000];

        /*
         * We could equally look for .svn and use svn add instead,
         * etc, etc, as required for other DVCS tools.
         */
        snprintf
        (
            command,
            sizeof(command),
            "aegis --new-file %s %s --verbose --nolog",
            c_fn,
            h_fn
        );
        explain_system_or_die(command);
    }
}


static void
search_line(const char *fn, int linum, const char *text)
{
    const char      *name;
    size_t          name_size;
    char            name2[100];
    const char      *defn;
    size_t          defn_size;
    char            defn2[100];

    while (isspace((unsigned char)*text))
        ++text;
    if (*text != '#')
        return;
    ++text;
    while (isspace((unsigned char)*text))
        ++text;
    if (0 != memcmp(text, "define", 6))
        return;
    text += 6;
    if (!isspace((unsigned char)*text))
        return;
    ++text;
    while (isspace((unsigned char)*text))
        ++text;

    /* we are looking at the name now */
    name = text;
    while (isalnum((unsigned char)*text) || *text == '_')
        ++text;
    name_size = text - name;
    if (name_size == 0)
        return;
    if (!isspace((unsigned char)*text))
        return;
    ++text;
    while (isspace((unsigned char)*text))
        ++text;

    /*
     * The text is now pointing at the value.  It should be one of _IO,
     * _IOR, _IORW, _IOW, _ION, _IORN, _IORWN or _IOWN, followed by open
     * paren.
     */
    defn = text;
    if (*text++ != '_')
        return;
    if (*text++ != 'I')
        return;
    if (*text++ != 'O')
        return;
    if (*text == 'R')
    {
        ++text;
        if (*text == 'W')
            ++text;
    }
    else if (*text == 'W')
        ++text;
    if (*text == 'N')
        ++text;
    if (*text != '(')
        return;

    /* find the end of the defintion */
    while (*text && *text != ')')
        ++text;
    defn_size = text - defn;

    /*
     * Looks like we have found an ioctl definition.
     */
    if (name_size > sizeof(name2) - 1)
        return;
    memcpy(name2, name, name_size);
    name2[name_size] = '\0';
    if (defn_size > sizeof(defn2) - 1)
        defn_size = sizeof(defn2) - 1;
    memcpy(defn2, defn, defn_size);
    defn2[defn_size] = '\0';
    if (explain_iocontrol_request_by_name(name2))
    {
        ++supported;
    }
    else
    {
        printf("%s: %d: %s\n", fn, linum, name2);
        ++unsupported;
        if (generate)
        {
            generate_code(name2, defn2, fn);
        }
    }
}


static void
search_file(const char *fn)
{
    FILE            *fp;
    int             linum;

    if (verbose >= 2)
        fprintf(stderr, "Checking \"%s\" file...\n", fn);
    fp = explain_fopen_or_die(fn, "r");
    linum = 0;
    for (;;)
    {
        size_t          line_len;
        char            line[2000];

        if (!explain_fgets_or_die(line, sizeof(line), fp))
            break;
        ++linum;
        line_len = strlen(line);
        if (line_len > 0 && line[line_len - 1] == '\n')
            line[line_len - 1] = '\0';
        search_line(fn, linum, line);
    }
    explain_fclose_or_die(fp);
    explain_fflush_or_die(stdout);
}


static int
cmp(const void *av, const void *bv)
{
    const char      *a;
    const char      *b;

    a = *(const char **)av;
    b = *(const char **)bv;
    return strcmp(a, b);
}


static void
search_dir(const char *path)
{
    DIR             *dp;
    char            **names;
    size_t          names_size;
    size_t          names_size_max;
    size_t          n;

    if (verbose)
        fprintf(stderr, "Checking \"%s\" directory...\n", path);

    /*
     * Read the names of all files (etc) in the directory.
     */
    dp = explain_opendir_or_die(path);
    names = 0;
    names_size = 0;
    names_size_max = 0;
    for (;;)
    {
        struct dirent *dep;

        dep = explain_readdir_or_die(dp);
        if (!dep)
            break;
        if (!strcmp(dep->d_name, ".") || !strcmp(dep->d_name, ".."))
            continue;
        if (names_size >= names_size_max)
        {
            size_t          new_names_size_max;
            char            **new_names;

            new_names_size_max = names_size_max * 2 + 16;
            new_names =
                explain_malloc_or_die(new_names_size_max * sizeof(names[0]));
            memcpy(new_names, names, names_size * sizeof(names[0]));
            free(names);
            names = new_names;
            names_size_max = new_names_size_max;
        }
        names[names_size++] = explain_strdup_or_die(dep->d_name);
    }
    explain_closedir_or_die(dp);

    /*
     * Sort the names of the files in the directory, so that we get
     * predicatable and repeatable results.
     */
    qsort(names, names_size, sizeof(names[0]), cmp);

    /*
     * Search each file and each directory.
     */
    for (n = 0; n < names_size; ++n)
    {
        struct stat     st2;
        char            *pp;
        char            path2[PATH_MAX];

        pp = path2;
        pp = explain_strendcpy(pp, path, path2 + sizeof(path2));
        pp = explain_strendcpy(pp, "/", path2 + sizeof(path2));
        pp = explain_strendcpy(pp, names[n], path2 + sizeof(path2));
        explain_lstat_or_die(path2, &st2);
        switch (st2.st_mode & S_IFMT)
        {
        case S_IFDIR:
            search_dir(path2);
            break;

        case S_IFREG:
            search_file(path2);
            break;

        default:
            /* silently ignore special files */
            break;
        }
    }

    /*
     * Release resources
     */
    for (n = 0; n < names_size; ++n)
        free(names[n]);
    free(names);
}


static void
search(const char *path)
{
    struct stat     st;

    explain_lstat_or_die(path, &st);
    switch (st.st_mode & S_IFMT)
    {
    case S_IFDIR:
        search_dir(path);
        break;

    case S_IFREG:
        search_file(path);
        break;

    default:
        explain_output_error_and_die("file \"%s\" not a directory", path);
    }
}


static void
usage(void)
{
    const char      *prog;

    prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s [ <option>... ][ <directory>... ]\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(1);
}


int
main(int argc, char **argv)
{
    explain_program_name_set(argv[0]);
    for (;;)
    {
        int             c;

        c = getopt(argc, argv, "gVv");
        if (c < 0)
            break;
        switch (c)
        {
        case 'g':
            ++generate;
            break;

        case 'V':
            explain_version_print();
            return 0;

        case 'v':
            ++verbose;
            break;

        default:
            usage();
        }
    }
    if (argc == optind)
        search_dir("/usr/include");
    else
    {
        while (optind < argc)
            search(argv[optind++]);
    }

    if (unsupported > 0)
    {
        explain_option_hanging_indent_set(4);
        explain_output_error
        (
            "Found %d supported and %d unsupported ioctl requests.  "
                "This is not a bug, but it does indicate where libexplain's "
                "ioctl support could be improved.  It is also possible there "
                "are more, but that they use a numeric definition, and can't "
                "be discovered by a dumb program.",
            supported,
            unsupported
        );
    }
    return 0;
}


/* vim: set ts=8 sw=4 et : */
