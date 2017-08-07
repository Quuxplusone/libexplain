/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2013 Peter Miller
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

#ifndef CODEGEN_IOCTL_SCAN_H
#define CODEGEN_IOCTL_SCAN_H

/**
  * The ioctl_scan_include function may be used to read a system include
  * file, and remember the ioctl requests it defines, placing them into
  * a suitably named catalogue file.
  *
  * After the file has been produced, it should be vetted by a human to
  * weed out the false positives, and put back the false negatives.
  *
  * @param pathname
  *    The name of the system include file to scan.
  */
void ioctl_scan_include(const char *pathname);

/**
  * The ioctl_scan_dir funvtion id usedto walk a directory tree, and
  * passing all .h files to ioctrl_scan_include
  */
void ioctl_scan_dir(const char *pathname);

/**
  * The ioctl_scan_generate function may be used to read a system include
  * file, and remember the ioctl requests it defines, placing them into
  * a suitably named catalogue file.
  *
  * @param pathname
  *    The name of the catalogue file created by the ioctl_scan_include
  *    function.
  */
void ioctl_scan_generate(const char *pathname);

#endif /* CODEGEN_IOCTL_SCAN_H */
/* vim: set ts=8 sw=4 et : */
