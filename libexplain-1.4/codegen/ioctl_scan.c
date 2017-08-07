/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010, 2013, 2014 Peter Miller
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
#include <libexplain/ac/ftw.h>
#include <libexplain/ac/regex.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/stat.h>

#include <libexplain/fclose.h>
#include <libexplain/fgets.h>
#include <libexplain/fopen.h>
#include <libexplain/malloc.h>
#include <libexplain/output.h>
#include <libexplain/sizeof.h>
#include <libexplain/string_list.h>

#include <codegen/aegis.h>
#include <codegen/catalogue.h>
#include <codegen/elastic_buffer.h>
#include <codegen/header.h>
#include <codegen/ioctl_scan.h>
#include <codegen/wrapper.h>


static void
regcomp_or_die(regex_t *preg, const char *regex, int cflags)
{
    int             errcode;

    errcode = regcomp(preg, regex, cflags);
    if (errcode != 0)
    {
        char            errbuf[1000];

        regerror(errcode, preg, errbuf, sizeof(errbuf));
        explain_output_error_and_die("regcomp(\"%s\"): %s", regex, errbuf);
    }
}


static int
regexec_or_die(const regex_t *preg, const char *string, size_t nmatch,
    regmatch_t *pmatch, int eflags)
{
    int             errcode;

    errcode = regexec(preg, string, nmatch, pmatch, eflags);
    switch (errcode)
    {
    case 0:
        return 1;

    case REG_NOMATCH:
        return 0;

    default:
        {
            char            errbuf[1000];

            regerror(errcode, preg, errbuf, sizeof(errbuf));
            explain_output_error_and_die("regexec: %s", errbuf);
        }
    }
}


static char *
mangle_include_file_name(const char *txt)
{
    size_t          len;
    char            *result;
    const char      *end;
    const char      *ip;
    char            *op;

    len = strlen(txt);
    if (len >= 2 && 0 == memcmp(txt + len - 2, ".h", 2))
        len -= 2;
    result = explain_malloc_or_die(len + 1);
    end = txt + len;
    ip = txt;
    op = result;
    while (ip < end)
    {
        unsigned char   c;

        c = *ip++;
        if (isupper(c))
            c = tolower(c);
        else if (!isalnum(c))
            c = '_';
        *op++ = c;
    }
    *op = '\0';
    return result;
}


void
ioctl_scan_include(const char *pathname)
{
    const char      *p;
    FILE            *ifp;
    regex_t         define_io;
    regex_t         define_hex;
    FILE            *ofp;
    char            ofn[1000];
    explain_string_list_t iocg;
    explain_string_list_t iocs;
    char            *mangled;
    int             line_number;
    size_t          j;

    p = pathname;
    if (0 != memcmp(pathname, "/usr/include/", 13))
    {
        explain_output_error_and_die("must be a system include file");
    }
    p += 13;

    /*
     * scan the file to see if has any ioctl defnitions,
     * and quietly ignore it if there are none.
     */
    explain_string_list_constructor(&iocg);
    explain_string_list_constructor(&iocs);
    regcomp_or_die
    (
        &define_io,
       "#[ \t]*define[ \t]+"
       "([A-Z][A-Z_0-9]*)"
       "[ \t]+"
       "(_IO[CRW]*[ \t]*\\([^)]*\\))",
        REG_EXTENDED
    );
    regcomp_or_die
    (
        &define_hex,
        "#[ \t]*define[ \t]+"
        "([A-Z_][0-9A-Za-z_]*IOC[A-Za-z_0-9]*)"
        "[ \t]+"
        "(0x[0-9A-Fa-f]+)",
        REG_EXTENDED
    );
    ifp = explain_fopen_or_die(pathname, "r");
    line_number = 0;
    for (;;)
    {
        char            line[1000];
        regmatch_t      match[3];

        if (!explain_fgets_or_die(line, sizeof(line), ifp))
            break;
        ++line_number;

        { char *nl = strchr(line, '\n'); if (nl) *nl = '\0'; }

        if (regexec_or_die(&define_hex, line, SIZEOF(match), match, 0))
        {
            explain_output_error
            (
                "%s:%d: %.*s 0x%08lX",
                p,
                line_number,
                (int)(match[1].rm_eo - match[1].rm_so),
                line + match[1].rm_so,
                strtoul(line + match[2].rm_so, 0, 0)
            );
            goto stash;
        }
        else if (regexec_or_die(&define_io, line, SIZEOF(match), match, 0))
        {
            size_t           len;
            const char       *str;
            size_t           len2;
            const char       *str2;

            explain_output_error
            (
                "%s:%d: %.*s %.*s",
                p,
                line_number,
                (int)(match[1].rm_eo - match[1].rm_so),
                line + match[1].rm_so,
                (int)(match[2].rm_eo - match[2].rm_so),
                line + match[2].rm_so
            );

            stash:
            len = match[1].rm_eo - match[1].rm_so;
            str = line + match[1].rm_so;

            len2 = match[2].rm_eo - match[2].rm_so;
            str2 = line + match[2].rm_so;
            if (len2 > 6 && 0 == memcmp(str2, "_IORW(", 6))
                explain_string_list_append_n(&iocs, str, len);
            else if (len2 > 5 && 0 == memcmp(str2, "_IOR(", 5))
                explain_string_list_append_n(&iocg, str, len);
            else if (len2 > 5 && 0 == memcmp(str2, "_IOW(", 5))
                explain_string_list_append_n(&iocs, str, len);
            else if (strnstr(str, "IOCS", len))
                explain_string_list_append_n(&iocs, str, len);
            else if (strnstr(str, "IOCG", len))
                explain_string_list_append_n(&iocg, str, len);
            else if (strnstr(str, "SET", len))
                explain_string_list_append_n(&iocs, str, len);
            else if (strnstr(str, "GET", len))
                explain_string_list_append_n(&iocg, str, len);
            else if (memcmp(str, "PIO", 3))
                explain_string_list_append_n(&iocs, str, len);
            else if (memcmp(str, "GIO", 3))
                explain_string_list_append_n(&iocg, str, len);
            else if (memcmp(str, "KDS", 3))
                explain_string_list_append_n(&iocs, str, len);
            else if (memcmp(str, "KDG", 3))
                explain_string_list_append_n(&iocg, str, len);
            else
                explain_string_list_append_n(&iocs, str, len);
        }
        else
        {
            /* no match */
        }
    }
    explain_fclose_or_die(ifp);
    regfree(&define_io);
    regfree(&define_hex);
    if (iocg.length == 0 && iocs.length == 0)
        return;
    explain_output_error("%s: iocg %zu, iocs %zu",
        pathname, iocg.length, iocs.length);

    /*
     * Now write the catalogue entry
     */
    mangled = mangle_include_file_name(p);

    snprintf(ofn, sizeof(ofn), "catalogue-ioctl/%s", mangled);
    free(mangled);
    mangled = 0;

    ofp = explain_fopen_or_die(ofn, "w");
    gpl_header(ofp, "# ");
    fprintf(ofp, "Include: %s\n", p);

    fprintf(ofp, "#\n");
    wrapper(ofp, "# ", "Requests that don't alter system state, device "
        "state or media state. These are safe to probe, and will not cause "
        "probems or damage.");
    fprintf(ofp, "Get:\n");
    explain_string_list_sort(&iocg);
    for (j = 0; j < iocg.length; ++j)
        fprintf(ofp, " %s\n", iocg.string[j]);

    fprintf(ofp, "#\n");
    wrapper(ofp, "# ", "These are requests that alter the system state.");
    fprintf(ofp, "Set:\n");
    explain_string_list_sort(&iocs);
    for (j = 0; j < iocs.length; ++j)
        fprintf(ofp, " %s\n", iocs.string[j]);

    explain_string_list_destructor(&iocg);
    explain_string_list_destructor(&iocs);

    explain_fclose_or_die(ofp);
}


static int
ends_with(const char *haystack, const char *needle)
{
    size_t haystack_size = strlen(haystack);
    size_t needle_size = strlen(needle);
    const char *haystack_end = haystack + haystack_size;
    return
        (
            needle_size < haystack_size
        &&
            !memcmp(haystack_end - needle_size, needle, needle_size)
        );
}


static int
walker(const char *pathname, const struct stat *st, int typeflag,
    struct FTW *ftwbuf)
{
    (void)typeflag;
    (void)ftwbuf;
    if (typeflag == FTW_D)
    {
       explain_output_error("walking %s\n", pathname);
#ifdef FTW_SKIP_SUBTREE
       if (!strcmp(pathname, ".."))
           return FTW_SKIP_SUBTREE;
       if (ends_with(pathname, "/.."))
           return FTW_SKIP_SUBTREE;
#endif
    }
    if (S_ISREG(st->st_mode) && ends_with(pathname , ".h"))
        ioctl_scan_include(pathname);
    return FTW_CONTINUE;
}


void
ioctl_scan_dir(const char *pathname)
{
    nftw(pathname, walker, 5, FTW_PHYS);
}


static char *
downcase(const char *text)
{
    size_t          len;
    char            *result;
    const char      *ip;
    char            *op;

    len = strlen(text) + 1;
    result = explain_malloc_or_die(len);
    ip = text;
    op = result;
    for (;;)
    {
        unsigned char c;

        c = *ip++;
        if (isupper(c))
            c = tolower(c);
        *op++ = c;
        if (!c)
            break;
    }
    return result;
}


static void
upcase_insitu(char *s)
{
    for (;;)
    {
        unsigned char   c;

        c = *s;
        if (!c)
            return;
        if (islower(c))
            *s = toupper(c);
        else if (!isalnum(c))
            *s = '_';
        ++s;
    }
}


void
ioctl_scan_generate(const char *pathname)
{
    const char      *include_file_name;
    catalogue_t     *cat;
    char            ofn[1000];
    const char      *getters_str;
    const char      *setters_str;
    explain_string_list_t getters_list;
    explain_string_list_t ioctl_list;
    FILE            *ofp;
    size_t          j;
    elastic_buffer_t buf;
    char            *func;

    cat = catalogue_open(pathname);
    include_file_name = catalogue_get(cat, "include");
    if (!include_file_name || !*include_file_name)
    {
        explain_output_error_and_die("%s: no \"Include\" field\n", pathname);
    }
    func = mangle_include_file_name(include_file_name);
    elastic_buffer_constructor(&buf);

    getters_str = catalogue_get(cat, "get");
    explain_string_list_constructor(&getters_list);
    explain_string_list_split(&getters_list, getters_str);
    explain_string_list_constructor(&ioctl_list);
    explain_string_list_split(&ioctl_list, getters_str);
    explain_string_list_sort(&getters_list);

    snprintf(ofn, sizeof(ofn), "ioctl-scan/scan/%s.c", func);
    aegis_new_file(ofn);

    ofp = explain_fopen_or_die(ofn, "w");
    gpl_header(ofp, " * ");
    fprintf(ofp, "\n");
    fprintf(ofp, "#include <libexplain/ac/%s>\n", include_file_name);
    fprintf(ofp, "#include <libexplain/ac/sys/ioctl.h>\n");
    fprintf(ofp, "\n");
    fprintf(ofp, "#include <ioctl-scan/report.h>\n");
    fprintf(ofp, "#include <ioctl-scan/scan/%s.h>\n", func);
    fprintf(ofp, "\n");
    fprintf(ofp, "\n");
    fprintf(ofp, "void\n");
    fprintf(ofp, "scan_%s(int fildes)\n", func);
    fprintf(ofp, "{\n");
    for (j = 0; j < getters_list.length; ++j)
    {
        const char *getter = getters_list.string[j];
        char *lc_getter = downcase(getter);
        fprintf(ofp, "#ifdef %s\n", getter);
        fprintf(ofp, "    report_semi_auto(fildes, %s);\n", getter);
        fprintf(ofp, "#endif\n");
        free(lc_getter);
    }
    fprintf(ofp, "}\n");
    explain_fclose_or_die(ofp);

    snprintf(ofn, sizeof(ofn), "ioctl-scan/scan/%s.h", func);
    aegis_new_file(ofn);
    ofp = explain_fopen_or_die(ofn, "w");
    upcase_insitu(ofn);
    gpl_header(ofp, " * ");
    fprintf(ofp, "\n");
    fprintf(ofp, "#ifndef %s\n", ofn);
    fprintf(ofp, "#define %s\n", ofn);

    fprintf(ofp, "\n");
    fprintf(ofp, "/**\n");
    elastic_buffer_rewind(&buf);
    elastic_buffer_puts(&buf, "The scan_");
    elastic_buffer_puts(&buf, func);
    elastic_buffer_puts
    (
        &buf,
        " function is used to attempt all of the (safe) ioctl requests "
        "defined in the <"
    );
    elastic_buffer_puts(&buf, include_file_name);
    elastic_buffer_puts
    (
        &buf,
        "> system include file, and print the results those that are "
        "successful.  Nothing is printed if an ioctl fails."
    );
    wrapper(ofp, "  * ", elastic_buffer_get(&buf));
    fprintf(ofp, "  *\n");
    fprintf(ofp, "  * @param fildes\n");
    fprintf(ofp, "  *     The file descriptor of the open device.\n");
    fprintf(ofp, "  */\n");
    fprintf(ofp, "void scan_%s(int fildes);\n", func);
    free(func);
    func = 0;
    fprintf(ofp, "\n");
    fprintf(ofp, "#endif /* %s */\n", ofn);
    explain_fclose_or_die(ofp);

    /*
     * Now we take all of the ioctl requests and generate
     * libexplain/iocontrol/%s.{h,c} for each of them.
     */
    setters_str = catalogue_get(cat, "set");
    explain_string_list_split(&ioctl_list, setters_str);
    explain_string_list_sort(&ioctl_list);

    for (j = 0; j < ioctl_list.length; ++j)
    {
        const char      *name;
        char            *lc_name;

        name = ioctl_list.string[j];
        lc_name = downcase(name);
        snprintf(ofn, sizeof(ofn), "libexplain/iocontrol/%s.h", lc_name);
        aegis_new_file(ofn);
        ofp = explain_fopen_or_die(ofn, "w");
        upcase_insitu(ofn);
        gpl_header(ofp, " * ");
        fprintf(ofp, "\n");
        fprintf(ofp, "#ifndef %s\n", ofn);
        fprintf(ofp, "#define %s\n", ofn);
        fprintf(ofp, "\n");

        fprintf(ofp, "#include <libexplain/iocontrol.h>\n");
        fprintf(ofp, "\n");

        fprintf(ofp, "/**\n");
        elastic_buffer_rewind(&buf);
        elastic_buffer_puts(&buf, "The explain_iocontrol_");
        elastic_buffer_puts(&buf, lc_name);
        elastic_buffer_puts
        (
            &buf,
            " global variable is used to store information about the "
        );
        elastic_buffer_puts(&buf, name);
        elastic_buffer_puts(&buf, " I/O control.");
        wrapper(ofp, "  * ", elastic_buffer_get(&buf));
        fprintf(ofp, "  *\n");
        fprintf(ofp, "  * @note\n");
        wrapper
        (
            ofp,
            "  *     ",
            "This information is not kept in a single table for all values, "
            "like every other set of constants, because (a) some values "
            "are ambiguous, and (b) the includes files have bugs making it "
            "impossible to include all of them in the same compilation unit."
        );
        fprintf(ofp, "  */\n");
        fprintf
        (
            ofp,
            "extern const explain_iocontrol_t explain_iocontrol_%s;\n",
            lc_name
        );

        fprintf(ofp, "\n");
        fprintf(ofp, "#endif /* %s */\n", ofn);
        explain_fclose_or_die(ofp);

        snprintf(ofn, sizeof(ofn), "libexplain/iocontrol/%s.c", lc_name);
        aegis_new_file(ofn);
        ofp = explain_fopen_or_die(ofn, "w");
        gpl_header(ofp, " * ");
        fprintf(ofp, "\n");
        fprintf(ofp, "#include <libexplain/ac/%s>\n", include_file_name);
        fprintf(ofp, "\n");
        fprintf(ofp, "#include <libexplain/buffer/%s.h>\n", lc_name);
        fprintf(ofp, "#include <libexplain/iocontrol/generic.h>\n");
        fprintf(ofp, "#include <libexplain/iocontrol/%s.h>\n", lc_name);
        fprintf(ofp, "\n");

        fprintf(ofp, "#ifdef %s\n", name);
        fprintf(ofp, "\n");
        fprintf(ofp, "\n");
        fprintf(ofp, "static void\n");
        wrapper_hang(ofp, "", "print_data(const explain_iocontrol_t *p, "
            "explain_string_buffer_t *sb, int errnum, int fildes, int request, "
            "const void *data)");
        fprintf(ofp, "{\n");
        fprintf(ofp, "    (void)p;\n");
        fprintf(ofp, "    (void)errnum;\n");
        fprintf(ofp, "    (void)fildes;\n");
        fprintf(ofp, "    (void)request;\n");
        fprintf(ofp, "    explain_buffer_%s(sb, data);\n", lc_name);
        fprintf(ofp, "}\n");
        fprintf(ofp, "\n");
        fprintf(ofp, "\n");
        fprintf(ofp, "static void\n");
        wrapper_hang(ofp, "", "print_explanation(const explain_iocontrol_t *p, "
            "explain_string_buffer_t *sb, int errnum, int fildes, int request, "
            "const void *data)");
        fprintf(ofp, "{\n");
        fprintf(ofp, "    switch (errnum)\n");
        fprintf(ofp, "    {\n");
        fprintf(ofp, "    default:\n");
        fprintf(ofp, "        explain_iocontrol_generic_print_explanation\n");
        fprintf(ofp, "        (\n");
        fprintf(ofp, "            p,\n");
        fprintf(ofp, "            sb,\n");
        fprintf(ofp, "            errnum,\n");
        fprintf(ofp, "            fildes,\n");
        fprintf(ofp, "            request,\n");
        fprintf(ofp, "            data\n");
        fprintf(ofp, "        );\n");
        fprintf(ofp, "        break;\n");
        fprintf(ofp, "    }\n");
        fprintf(ofp, "}\n");

        fprintf(ofp, "\n");
        fprintf(ofp, "\n");
        fprintf(ofp, "static void\n");
        wrapper_hang(ofp, "", "print_data_returned("
            "const explain_iocontrol_t *p, explain_string_buffer_t *sb, "
            "int errnum, int fildes, int request, const void *data)");
        fprintf(ofp, "{\n");
        fprintf(ofp, "    (void)p;\n");
        fprintf(ofp, "    (void)errnum;\n");
        fprintf(ofp, "    (void)fildes;\n");
        fprintf(ofp, "    (void)request;\n");
        fprintf(ofp, "    explain_buffer_%s(sb, data);\n", lc_name);
        fprintf(ofp, "}\n");

        fprintf(ofp, "\n");
        fprintf(ofp, "\n");
        fprintf(ofp, "const explain_iocontrol_t explain_iocontrol_%s =\n",
            lc_name);
        fprintf(ofp, "{\n");
        fprintf(ofp, "    \"%s\", /* name */\n", name);
        fprintf(ofp, "    %s, /* value */\n", name);
        fprintf(ofp, "    0, /* disambiguate */\n");
        fprintf(ofp, "    0, /* print_name */\n");
        fprintf(ofp, "    0, /* print_data */\n");
        fprintf(ofp, "    0, /* print_explanation */\n");
        fprintf(ofp, "    0, /* print_data_returned */\n");
        fprintf(ofp, "    sizeof(struct %s), /* data_size */\n", lc_name);
        fprintf(ofp, "    __FILE__,\n");
        fprintf(ofp, "    __LINE__,\n");
        fprintf(ofp, "};\n");
        fprintf(ofp, "\n");
        fprintf(ofp, "#else /* ndef %s */\n", name);
        fprintf(ofp, "\n");
        fprintf(ofp, "const explain_iocontrol_t explain_iocontrol_%s =\n",
            lc_name);
        fprintf(ofp, "{\n");
        fprintf(ofp, "    0, /* name */\n");
        fprintf(ofp, "    0, /* value */\n");
        fprintf(ofp, "    0, /* disambiguate */\n");
        fprintf(ofp, "    0, /* print_name */\n");
        fprintf(ofp, "    0, /* print_data */\n");
        fprintf(ofp, "    0, /* print_explanation */\n");
        fprintf(ofp, "    0, /* print_data_returned */\n");
        fprintf(ofp, "    0, /* data_size */\n");
        fprintf(ofp, "    __FILE__,\n");
        fprintf(ofp, "    __LINE__,\n");
        fprintf(ofp, "};\n");
        fprintf(ofp, "\n");
        fprintf(ofp, "#endif /* %s */\n", name);
        explain_fclose_or_die(ofp);
    }

    snprintf(ofn, sizeof(ofn), "libexplain/iocontrol/table.c");
    aegis_copy_file(ofn);
    ofp = explain_fopen_or_die(ofn, "a");
    for (j = 0; j < ioctl_list.length; ++j)
    {
        const char      *name;
        char            *lc_name;

        name = ioctl_list.string[j];
        lc_name = downcase(name);
        fprintf(ofp, "#include <libexplain/iocontrol/%s.h>\n", lc_name);
    }
    for (j = 0; j < ioctl_list.length; ++j)
    {
        const char      *name;
        char            *lc_name;

        name = ioctl_list.string[j];
        lc_name = downcase(name);
        fprintf(ofp, "    &explain_iocontrol_%s,\n", lc_name);
    }
    explain_fclose_or_die(ofp);

    elastic_buffer_destructor(&buf);
}


/* vim: set ts=8 sw=4 et : */
