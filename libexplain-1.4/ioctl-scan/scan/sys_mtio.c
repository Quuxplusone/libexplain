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

#include <libexplain/ac/sys/mtio.h>
#include <libexplain/ac/sys/ioctl.h>

#include <ioctl-scan/report.h>
#include <ioctl-scan/scan/sys_mtio.h>


void
scan_sys_mtio(int fildes)
{
#ifdef MTIOCGET
    report_semi_auto(fildes, MTIOCGET);
#endif
#ifdef MTIOCGETCONFIG
    report_semi_auto(fildes, MTIOCGETCONFIG);
#endif
#ifdef MTIOCPOS
    report_semi_auto(fildes, MTIOCPOS);
#endif
}
