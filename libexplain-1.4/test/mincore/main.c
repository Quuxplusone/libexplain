/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2012 Peter Miller
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

#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/mman.h>
#include <libexplain/ac/unistd.h>


static int
mincore_says_is_in_core(void *data, size_t data_size)
{
    size_t          page_size;
    size_t          lo_pages;
    size_t          hi_pages;
    void            *lo;
    void            *hi;
    size_t          bsize;
    size_t          vec_bytes;
    unsigned char   vec[100];

    page_size = getpagesize();

    lo_pages = (size_t)data / page_size;
    hi_pages = ((size_t)data + data_size + page_size - 1) / page_size;
    lo = (void *)(lo_pages * page_size);
    hi = (void *)(hi_pages * page_size);
    bsize = (char *)hi - (char *)lo;
    vec_bytes = hi_pages - lo_pages;
    if (vec_bytes > sizeof(vec))
        return 0;
    return (mincore(lo, bsize, (void *)vec) >= 0);
}


static int
mincore_detects_stack_ok(void)
{
    char            dummy[1000];

    /* set memory to force copy-on-write, of necessary */
    memset(dummy, 0xAA, sizeof(dummy));
    return mincore_says_is_in_core(dummy, sizeof(dummy));
}


static int
mincore_detects_heap_ok(void)
{
    size_t          dummy_size;
    char            *dummy;
    int             result;

    dummy_size = 1000;
    dummy = malloc(dummy_size);
    /* set memory to force copy-on-write, of necessary */
    memset(dummy, 0xAA, dummy_size);
    result = mincore_says_is_in_core(dummy, dummy_size);
    free(dummy);
    return result;
}


int
main(int argc, char **argv)
{
    int             ok;

    (void)argc;
    (void)argv;

    /*
     * The mincore function is highly variable in implementation between
     * Unix implementations, and even from one release to another of a
     * single brand of Unix.  So we have to check.
     *
     *    "[The mincore function] is probably still more os-specific and, in
     *    fact, architecture specific, than you think.  This deep in the VM live
     *    daemons." -- Daniel Pittman <daniel@rimspace.net>
     */
    ok = mincore_detects_stack_ok() && mincore_detects_heap_ok();

    return (ok ? EXIT_SUCCESS : EXIT_FAILURE);
}


/* vim: set ts=8 sw=4 et : */
