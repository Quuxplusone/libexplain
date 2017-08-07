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

#include <libexplain/ac/stdint.h> /* order matters for Jaunty */
#include <libexplain/ac/linux/ata.h>
#include <libexplain/ac/linux/hdreg.h>
#include <libexplain/ac/sys/ioctl.h>

#include <libexplain/iocontrol/hdio_getgeo.h>
#include <libexplain/iocontrol/hdio_get_identity.h>
#include <libexplain/string_buffer.h>

#include <ioctl-scan/report.h>
#include <ioctl-scan/scan/linux_hdreg.h>


void
scan_linux_hdreg(int fildes)
{
    (void)fildes;
#ifdef HDIO_GETGEO
    report_semi_auto(fildes, HDIO_GETGEO);
#endif
#ifdef HDIO_GET_32BIT
    report_semi_auto(fildes, HDIO_GET_32BIT);
#endif
#ifdef HDIO_GET_ACOUSTIC
    report_semi_auto(fildes, HDIO_GET_ACOUSTIC);
#endif
#ifdef HDIO_GET_ADDRESS
    report_semi_auto(fildes, HDIO_GET_ADDRESS);
#endif
#ifdef HDIO_GET_BUSSTATE
    report_semi_auto(fildes, HDIO_GET_BUSSTATE);
#endif
#ifdef HDIO_GET_DMA
    report_semi_auto(fildes, HDIO_GET_DMA);
#endif
#ifdef HDIO_GET_IDENTITY
    report_semi_auto(fildes, HDIO_GET_IDENTITY);
#endif
#ifdef HDIO_GET_MULTCOUNT
    report_semi_auto(fildes, HDIO_GET_MULTCOUNT);
#endif
#ifdef HDIO_GET_NICE
    report_semi_auto(fildes, HDIO_GET_NICE);
#endif
#ifdef HDIO_GET_NOWERR
    report_semi_auto(fildes, HDIO_GET_NOWERR);
#endif
#ifdef HDIO_GET_QDMA
    report_semi_auto(fildes, HDIO_GET_QDMA);
#endif
#ifdef HDIO_GET_UNMASKINTR
    report_semi_auto(fildes, HDIO_GET_UNMASKINTR);
#endif
#ifdef HDIO_GET_WCACHE
    report_semi_auto(fildes, HDIO_GET_WCACHE);
#endif
}
