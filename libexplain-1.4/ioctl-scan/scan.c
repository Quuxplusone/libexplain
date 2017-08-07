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

#include <libexplain/ac/fcntl.h>

#include <libexplain/open.h>
#include <libexplain/close.h>

#include <ioctl-scan/scan.h>
#include <ioctl-scan/scan/asm_ioctls.h>
#include <ioctl-scan/scan/linux_cdrom.h>
#include <ioctl-scan/scan/linux_ext2_fs.h>
#include <ioctl-scan/scan/linux_fs.h>
#include <ioctl-scan/scan/linux_hdreg.h>
#include <ioctl-scan/scan/linux_lp.h>
#include <ioctl-scan/scan/linux_vt.h>
#include <ioctl-scan/scan/sys_mtio.h>


void
scan_block_device(const char *pathname)
{
    int             fildes;

    fildes = explain_open_or_die(pathname, O_RDONLY + O_NONBLOCK, 0);
    scan_asm_ioctls(fildes);
    scan_linux_cdrom(fildes);
    scan_linux_ext2_fs(fildes);
    scan_linux_fs(fildes);
    scan_linux_hdreg(fildes);
    scan_linux_lp(fildes);
    scan_linux_vt(fildes);
    scan_sys_mtio(fildes);
    explain_close_or_die(fildes);
}
