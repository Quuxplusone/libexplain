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

#include <libexplain/ac/errno.h>
#include <libexplain/ac/fcntl.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/sys/ioctl.h> /* for ioctl() except Solaris */
#include <libexplain/ac/unistd.h> /* for ioctl() on Solaris */

#include <libexplain/iocontrol/generic.h>
#include <libexplain/open.h>
#include <libexplain/close.h>

#include <ioctl-scan/probe.h>


static void
report(int fildes, int request, void *data)
{
    const explain_iocontrol_t *p;
    char            name[50];

    p = explain_iocontrol_find_by_number(fildes, request, data);
    explain_iocontrol_fake_syscall_name(name, sizeof(name), p, request);
    printf("%s\n", name);
}


static inline int
size_incr(int size)
{
    if (size < 0)
        return 0;
    if (size == 1)
        return 2;
    if (size == 2 || size == 3)
        return 4;
    return ((size + 4) & -4);
}


#ifndef _IOC_TYPEMASK
#define _IOC_TYPEMASK 255
#endif

#ifndef _IOC_NRMASK
#define _IOC_NRMASK 255
#endif

#ifndef _IOC_DIRMASK
# ifdef IOC_DIRMASK
#  define _IOC_DIRMASK (int)(IOC_DIRMASK)
# else
#  define _IOC_DIRMASK 3
# endif
#endif

#ifndef _IOC_SIZEMASK
# ifdef IOCPARM_MASK
#  define _IOC_SIZEMASK IOCPARM_MASK
# else
#  define _IOC_SIZEMASK 0x1FFF
# endif
#endif

#ifndef _IOC
#define _IOC(dir, type, nr, size) \
    (((dir)*0) + ((type) << 8) + (nr) + ((size)*0))
#endif

#define minbit(x) ((x) & -(x))


void
probe(const char *pathname)
{
    int             fildes;
    void            *data;
    int             type;
    int             bol;

    data = (void *)0x12345;
    fildes = explain_open_or_die(pathname, O_RDONLY | O_NONBLOCK, 0);
    bol = 1;
    for (type = 0; type <= _IOC_TYPEMASK; ++type)
    {
        int             nr;

        fprintf(stderr, "\r%3d of %d ", type, _IOC_TYPEMASK + 1);
        fflush(stderr);
        bol = 0;
        for (nr = 0; nr <= _IOC_NRMASK; ++nr)
        {
            int             dir;

            for (dir = 0; dir <= _IOC_DIRMASK; dir += minbit(_IOC_DIRMASK))
            {
                int             size;

                for (size = 0; size <= _IOC_SIZEMASK; size = size_incr(size))
                {
                    int             request;

                    request = _IOC(dir, type, nr, size);
                    if (ioctl(fildes, request, data) >= 0)
                    {
                        /* This one worked.  Hope it didn't make a mess */
                        if (!bol)
                        {
                            fprintf(stderr, "\r\33[K");
                            bol = 1;
                        }
                        report(fildes, request, data);
                    }
                    else
                    {
                        switch (errno)
                        {
                        case ENOSYS:
#if defined(EOPNOTSUPP) && (ENOSYS != EOPNOTSUPP)
                        case EOPNOTSUPP:
#endif
                        case ENOTTY:
#ifdef ENOIOCTLCMD
                        case ENOIOCTLCMD:
#endif
#ifdef ENOIOCTL
                        case ENOIOCTL:
#endif
                            /* definitely does not exist */
                            break;

                        case EINVAL:
                            /*
                             * Some (old) broken device drivers report
                             * these instead of ENOTTY like they are
                             * supposed to.  Assume it means "no".
                             */
                            break;

                        case EFAULT:
                        case EPERM:
                        case EACCES:
                        default:
                            /* definitely exists */
                            if (!bol)
                            {
                                fprintf(stderr, "\r\33[K");
                                bol = 1;
                            }
                            report(fildes, request, data);
                            break;
                        }
                    }
                }
            }
        }
    }
    explain_close_or_die(fildes);
    if (!bol)
        fprintf(stderr, "\r\33[K");
}
