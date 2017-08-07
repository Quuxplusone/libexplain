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

#include <libexplain/ac/sys/types.h> /* must be first */
#include <libexplain/ac/linux/hayesesp.h>
#include <libexplain/ac/linux/serial.h>
#include <libexplain/ac/linux/termios.h>
#ifndef HAVE_LINUX_TERMIOS_H
#include <libexplain/ac/termios.h>
#include <libexplain/ac/sys/ioctl.h>
#endif

#include <ioctl-scan/report.h>
#include <ioctl-scan/scan/asm_ioctls.h>


void
scan_asm_ioctls(int fildes)
{
#ifdef TCGETA
    report_semi_auto(fildes, TCGETA);
#endif
#ifdef TCGETS
    report_semi_auto(fildes, TCGETS);
#endif
#ifdef TCGETS2
#ifdef HAVE_LINUX_TERMIOS_H
    report_semi_auto(fildes, TCGETS2);
#endif
#endif
#ifdef TCGETX
#ifdef HAVE_LINUX_TERMIOS_H
    report_semi_auto(fildes, TCGETX);
#endif
#endif
#ifdef TIOCGETD
    report_semi_auto(fildes, TIOCGETD);
#endif
#ifdef TIOCGHAYESESP
#ifdef HAVE_LINUX_TERMIOS_H
    report_semi_auto(fildes, TIOCGHAYESESP);
#endif
#endif
#ifdef TIOCGICOUNT
#ifdef HAVE_LINUX_SERIAL_H
    report_semi_auto(fildes, TIOCGICOUNT);
#endif
#endif
#ifdef TIOCGLCKTRMIOS
    report_semi_auto(fildes, TIOCGLCKTRMIOS);
#endif
#ifdef TIOCGPGRP
    report_semi_auto(fildes, TIOCGPGRP);
#endif
#ifdef TIOCGPTN
    report_semi_auto(fildes, TIOCGPTN);
#endif
#if 0 /* def TIOCGRS485 */
    report_semi_auto(fildes, TIOCGRS485);
#endif
#ifdef TIOCGSERIAL
#ifdef HAVE_LINUX_SERIAL_H
    report_semi_auto(fildes, TIOCGSERIAL);
#endif
#endif
#ifdef TIOCGSID
    report_semi_auto(fildes, TIOCGSID);
#endif
#ifdef TIOCGSOFTCAR
    report_semi_auto(fildes, TIOCGSOFTCAR);
#endif
#ifdef TIOCGWINSZ
    report_semi_auto(fildes, TIOCGWINSZ);
#endif
#ifdef TIOCMGET
    report_semi_auto(fildes, TIOCMGET);
#endif
}
