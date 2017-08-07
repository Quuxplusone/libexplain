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

#include <libexplain/ac/assert.h>
#include <libexplain/ac/errno.h>
#include <libexplain/ac/getopt.h>
#include <libexplain/ac/iconv.h>
#include <libexplain/ac/libintl.h> /* textdomain */
#include <libexplain/ac/locale.h> /* setlocale */
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/fread.h>
#include <libexplain/freopen.h>
#include <libexplain/fwrite.h>
#include <libexplain/iconv.h>
#include <libexplain/iconv_close.h>
#include <libexplain/iconv_open.h>
#include <libexplain/output.h>
#include <libexplain/program_name.h>
#include <libexplain/version_print.h>


static void
usage(void)
{
    const char *prog;

    prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s <to-code> <from-code>\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(EXIT_FAILURE);
}


#define ICONV_FAIL ((size_t)(-1))


int
main(int argc, char **argv)
{
    const char      *to_code;
    const char      *from_code;
    iconv_t         cd;
    char            *ip;
    size_t          isize;
    char            *op;
    size_t          osize;
    size_t          n;
    int             omit_invalid;
    char            ibuf[1 << 12];
    char            obuf[sizeof(ibuf)];

    /*
     * Code sets to convert from and to respectively.  An empty string as the
     * default causes the 'iconv_open' function to look up the charset of the
     * currently selected locale and use it.
     */
    from_code = "";
    to_code = "";

    omit_invalid = 0;
    for (;;)
    {
        static const struct option options[] =
        {
            { "from-code", 1, 0, 'f' },
            { "to-code", 1, 0, 't' },
            { "output", 1, 0, 'o' },
            { "version", 0, 0, 'V' },
            { 0, 0, 0, 0 }
        };

        int c = getopt_long(argc, argv, "cf:o:t:V", options, 0);
        if (c == EOF)
            break;
        switch (c)
        {
        case 'c':
            /* omit invalid characters from input */
            omit_invalid = 1;
            break;

        case 'f':
            from_code = optarg;
            break;

        case 'o':
            explain_freopen_or_die(optarg, "w", stdout);
            break;

        case 't':
            to_code = optarg;
            break;

        case 'V':
            explain_version_print();
            return EXIT_SUCCESS;

        default:
            usage();
        }
    }
    switch (argc - optind)
    {
    case 0:
        break;

    case 1:
        explain_freopen_or_die(argv[optind], "r", stdin);
        break;

    default:
        usage();
    }

    /* Set locale via LC_ALL.  */
    setlocale(LC_ALL, "");

    /* Set the text message domain.  */
    textdomain("libexplain");

    cd = explain_iconv_open_or_die(to_code, from_code);

    ip = ibuf;
    isize = 0;
    op = obuf;
    osize = sizeof(obuf);
    for (;;)
    {
        if (isize == 0)
        {
            isize = explain_fread_or_die(ibuf, 1, sizeof(ibuf), stdin);
            if (isize == 0)
            {
                /* end of input */
                break;
            }
            ip = ibuf;
        }

        n = iconv(cd, &ip, &isize, &op, &osize);
        if (n == ICONV_FAIL)
        {
            switch (errno)
            {
            case EILSEQ:
                /*
                 * 1. if an invalid multibyte sequence is encountered in
                 * the input.  It sets errno to EILSEQ and
                 * returns ICONV_FAIL.  *inbuf is left pointing to the
                 * beginning of the invalid multibyte sequence.
                 */
                {
                    unsigned char c;
                    assert(isize > 0);
                    c = *ip;
                    if (omit_invalid)
                    {
                        ++ip;
                        --isize;
                    }
                    else
                    {
                        if (op > obuf)
                        {
                            explain_fwrite_or_die(obuf, 1, op - obuf, stdout);
                            op = obuf;
                            osize = sizeof(obuf);
                        }
                        *op++ = c;
                        --osize;
                    }
                }
                break;

            case EINVAL:
                /*
                 * 3. An incomplete multibyte sequence is encountered
                 * in the input, and the input byte sequence terminates
                 * after it.  In this case it sets errno to EINVAL and
                 * returns ICONV_FAIL. *inbuf is left pointing to the
                 * beginning of the incomplete multibyte sequence.
                 */
                {
                    size_t          nbytes;

                    if (op > obuf)
                    {
                        explain_fwrite_or_die(obuf, 1, op - obuf, stdout);
                        op = obuf;
                        osize = sizeof(obuf);
                    }

                    /* shift it all down to the start of the buffer */
                    nbytes = isize;
                    memmove(ibuf, ip, nbytes);
                    ip = ibuf;
                    isize = nbytes;

                    /* now read more input */
                    isize +=
                        explain_fread_or_die
                        (
                            ibuf + nbytes,
                            1,
                            sizeof(ibuf) - nbytes,
                            stdin
                        );
                }
                break;

            case E2BIG:
                /*
                 * 4. The output buffer has no more room for the next
                 * converted character.
                 */
                explain_fwrite_or_die(obuf, 1, op - obuf, stdout);
                op = obuf;
                osize = sizeof(obuf);
                break;

            default:
                /* something not mentioned in the man page */
                explain_output_error_and_die
                (
                    "%s",
                    explain_iconv(cd, &ip, &isize, &op, &osize)
                );
                break;
            }
        }
        else
        {
            if (op > obuf)
            {
                explain_fwrite_or_die(obuf, 1, op - obuf, stdout);
                op = obuf;
                osize = sizeof(obuf);
            }
        }
    }

    /* make sure we have enough room to reset the shift state */
    if (op > obuf)
    {
        explain_fwrite_or_die(obuf, 1, op - obuf, stdout);
        op = obuf;
        osize = sizeof(obuf);
    }

    /* set the conversion state to the initial state */
    n = iconv(cd, NULL, NULL, &op, &osize);
    if (n == ICONV_FAIL)
    {
        switch (errno)
        {
        case EILSEQ:
            /*
             * Invalid multi-byte sequence in input.
             * But there was no input.  Ke?
             */
            assert(!"invalid multi-byte sequence in input, "
                "but no input supplied");
            break;

        case EINVAL:
            assert(!"multi-byte sequence spans end-of-input, "
                "but no input supplied");
            break;

        case E2BIG:
            assert(!"this should not happen, output buffer is huge");
            break;

        default:
            assert(!"no idea");
            break;
        }
    }
    explain_fwrite_or_die(obuf, 1, sizeof(obuf) - osize, stdout);

    explain_iconv_close_or_die(cd);
    return EXIT_SUCCESS;
}


/* vim: set ts=8 sw=4 et : */
