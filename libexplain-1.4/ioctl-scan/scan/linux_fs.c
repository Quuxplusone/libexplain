/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but
 * WITHOUT ANY WARRANTY; without even the implied warranty
 * ofMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNULesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/linux/fs.h>
#include <libexplain/ac/sys/ioctl.h>

#include <ioctl-scan/report.h>
#include <ioctl-scan/scan/linux_fs.h>


void
scan_linux_fs(int fildes)
{
#ifndef HAVE_LINUX_FS_H
    (void)fildes;
#endif
#ifdef BLKROGET
    report_semi_auto(fildes, BLKROGET);
#endif
#ifdef BLKGETSIZE
    report_semi_auto(fildes, BLKGETSIZE);
#endif
#ifdef BLKRAGET
    report_semi_auto(fildes, BLKRAGET);
#endif
#ifdef BLKFRAGET
    report_semi_auto(fildes, BLKFRAGET);
#endif
#ifdef BLKSECTGET
    report_semi_auto(fildes, BLKSECTGET);
#endif
#ifdef BLKSSZGET
    report_semi_auto(fildes, BLKSSZGET);
#endif
#ifdef FIGETBSZ
    report_semi_auto(fildes, FIGETBSZ);
#endif
#ifdef BLKELVGET
    report_semi_auto(fildes, BLKELVGET);
#endif
#ifdef BLKBSZGET
    report_semi_auto(fildes, BLKBSZGET);
#endif
#ifdef BLKGETSIZE64
    report_semi_auto(fildes, BLKGETSIZE64);
#endif
#ifdef FS_IOC_GETFLAGS
    report_semi_auto(fildes, FS_IOC_GETFLAGS);
#endif
#ifdef FS_IOC_GETVERSION
    report_semi_auto(fildes, FS_IOC_GETVERSION);
#endif
#ifdef FS_IOC32_GETFLAGS
    report_semi_auto(fildes, FS_IOC32_GETFLAGS);
#endif
#ifdef FS_IOC32_GETVERSION
    report_semi_auto(fildes, FS_IOC32_GETVERSION);
#endif
}
