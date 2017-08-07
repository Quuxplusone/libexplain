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

#ifndef IOCTL_SCAN_IOCONTROL_H
#define IOCTL_SCAN_IOCONTROL_H

/**
  * The iocontrol function may be used to
  * call ioctl without including <sys/ioctl.h>
  * because that has conflicts with <linux/termios.h>
  *
  * @param fildes
  *    The file descriptor, exactly as to be passed to ioctl
  * @param request
  *    The request, exactly as to be passed to ioctl
  * @param data
  *    The data, exactly as to be passed to ioctl
  * @returns
  *    whatever ioctl returns
  */
int iocontrol(int fildes, int request, void *data);

#endif /* IOCTL_SCAN_IOCONTROL_H */
