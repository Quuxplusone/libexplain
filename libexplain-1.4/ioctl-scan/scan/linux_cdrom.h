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

#ifndef IOCTL_SCAN_SCAN_LINUX_CDROM_H
#define IOCTL_SCAN_SCAN_LINUX_CDROM_H

/**
  * The scan_linux_cdrom function is used to attempt all of the (safe)
  * ioctl requests defined in the <linux/cdrom.h> system include file, and
  * print the results those that are successful. Nothing is printed if an
  * ioctl fails.
  *
  * @param fildes
  *     The file descriptor of the open device.
  */
void scan_linux_cdrom(int fildes);

#endif /* IOCTL_SCAN_SCAN_LINUX_CDROM_H */
