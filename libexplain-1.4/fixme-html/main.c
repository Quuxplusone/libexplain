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

#include <libexplain/ac/ctype.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/fclose.h>
#include <libexplain/fgets.h>
#include <libexplain/fopen.h>
#include <libexplain/freopen.h>
#include <libexplain/program_name.h>
#include <libexplain/version_print.h>


static int      initialised;
static int      fixme_count;


static void
begin(void)
{
    if (initialised)
        return;
    initialised = 1;

    printf("<html>\n");
    printf("<body>\n");
    printf("<h1> Fix Me, Please! </h1>\n");
    printf("The following notes were automatically extracted from\n");
    printf("the source files, at places where the code is marked\n");
    printf("&ldquo;<tt>FIXME:</tt>&rdquo;.\n");
    printf("<table>\n");
    printf("<tr><th>Filename</th><th>Line</th><th>Comment</th></tr>\n");
}


static void
process(const char *filename)
{
    FILE            *fp;
    int             line_number;

    begin();
    fp = explain_fopen_or_die(filename, "r");
    line_number = 0;
    for (;;)
    {
        char            line[1000];
        char            *cp;

        if (!explain_fgets_or_die(line, sizeof(line), fp))
            break;
        ++line_number;

        cp = strstr(line, "FIXME:");
        if (cp)
        {
            char            *ep;
            const char      *fn;
            const char      *bp;

            cp += 6;
            while (*cp && isspace((unsigned char)*cp))
                ++cp;
            ep = cp + strlen(cp);
            while (ep > cp)
            {
                if (isspace((unsigned char)ep[-1]))
                {
                    --ep;
                    continue;
                }
                if (ep - cp >= 2 && ep[-2] == '*' && ep[-1] == '/')
                {
                    ep -= 2;
                    continue;
                }
                break;
            }

            fn = filename;
            bp = fn;
            while (bp[0] == 'b' && bp[1] == 'l')
                bp += 2;
            if (*bp == '/')
                fn = bp + 1;

            if
            (
                0 != strcmp(fn, "fixme-html/main.c")
            &&
                0 != strcmp(fn, "web-src/index.html")
            )
            {
                printf("<tr><td valign=\"top\" align=\"right\" >%s</td>\n", fn);
                printf
                (
                    "    <td valign=\"top\" align=\"right\" >%d:</td>\n",
                    line_number
                );
                printf
                (
                    "    <td valign=\"top\" >%.*s</td></tr>\n",
                    (int)(ep - cp),
                    cp
                );
                printf("    </tr>\n");
                ++fixme_count;
            }
        }
    }
    explain_fclose_or_die(fp);
}


static void
end(void)
{
    printf("</table>\n");
    printf("<p>\n");
    printf
    (
        "Found %d places in the code marked &ldquo;FIXME:&rdquo;.\n",
        fixme_count
    );
    printf("</body>\n");
    printf("</html>\n");
}


static void
process_list(const char *filename)
{
    FILE            *fp;

    fp = stdin;
    if (0 != strcmp(filename, "-"))
        fp = explain_fopen_or_die(filename, "r");
    for (;;)
    {
        size_t          len;
        char            line[1000];

        if (!explain_fgets_or_die(line, sizeof(line), fp))
            break;
        len = strlen(line);
        if (line[len - 1] == '\n')
            line[len - 1] = '\0';
        process(line);
    }
    if (fp != stdin)
        explain_fclose_or_die(fp);
}


static void
usage(void)
{
    const char      *prog;

    prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s [ <option>... ] <filename>...\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(1);
}


int
main(int argc, char **argv)
{
    for (;;)
    {
        int c = getopt(argc, argv, "F:o:V");
        if (c == EOF)
            break;
        switch (c)
        {
        case 'o':
            explain_freopen_or_die(optarg, "w", stdout);
            break;

        case 'F':
            process_list(optarg);
            break;

        case 'V':
            explain_version_print();
            return 0;

        default:
            usage();
        }
    }

    while (optind < argc)
        process(argv[optind++]);
    if (!initialised)
    {
        fprintf(stderr, "%s: no files named\n", argv[0]);
        exit(1);
    }
    end();
    explain_fclose_or_die(stdout);
    return 0;
}
