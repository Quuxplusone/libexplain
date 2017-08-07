/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010 Peter Miller
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

#include <libexplain/ac/linux/lp.h>
#include <libexplain/ac/sys/ioctl.h>

#include <ioctl-scan/report.h>
#include <ioctl-scan/scan/linux_lp.h>


void
scan_linux_lp(int fildes)
{
    (void)fildes;
#ifdef LPGETFLAGS
    report_semi_auto(fildes, LPGETFLAGS);
#endif
#ifdef LPGETIRQ
    report_semi_auto(fildes, LPGETIRQ);
#endif
#ifdef LPGETSTATS
    report_semi_auto(fildes, LPGETSTATS);
#endif
#ifdef LPGETSTATUS
    report_semi_auto(fildes, LPGETSTATUS);
#endif
}
