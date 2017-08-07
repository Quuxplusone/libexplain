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

#ifndef IOCTL_SCAN_REPORT_H
#define IOCTL_SCAN_REPORT_H

#include <libexplain/ac/stdint.h>

/**
  * The report_int function may be used to print a line of results for
  * an ioctl request.
  *
  * @param request
  *    The name of the ioctl request, suitable for copy-and-paste.
  * @param name
  *    The human readable English name of the result returned by the request.
  * @param value
  *    The value returned by the request.
  * @param units
  *    The units of the request, or NULL if irrelevant.
  */
void report_int(const char *request, const char *name, int value,
    const char *units);

/**
  * The report_bool function may be used to print a line of results for
  * an ioctl request.
  *
  * @param request
  *    The name of the ioctl request, suitable for copy-and-paste.
  * @param name
  *    The human readable English name of the result returned by the request.
  * @param value
  *    The value returned by the request.
  * @param units
  *    The units of the request, or NULL if irrelevant.
  */
void report_bool(const char *request, const char *name, int value);

/**
  * The report_long function may be used to print a line of results for
  * an ioctl request.
  *
  * @param request
  *    The name of the ioctl request, suitable for copy-and-paste.
  * @param name
  *    The human readable English name of the result returned by the request.
  * @param value
  *    The value returned by the request.
  * @param units
  *    The units of the request, or NULL if irrelevant.
  */
void report_long(const char *request, const char *name, long value,
    const char *units);

/**
  * The report_ushort function may be used to print a line of results for
  * an ioctl request.
  *
  * @param request
  *    The name of the ioctl request, suitable for copy-and-paste.
  * @param name
  *    The human readable English name of the result returned by the request.
  * @param value
  *    The value returned by the request.
  * @param units
  *    The units of the request, or NULL if irrelevant.
  */
void report_ushort(const char *request, const char *name, unsigned short value,
    const char *units);

/**
  * The report_uint64 function may be used to print a line of results for
  * an ioctl request.
  *
  * @param request
  *    The name of the ioctl request, suitable for copy-and-paste.
  * @param name
  *    The human readable English name of the result returned by the request.
  * @param value
  *    The value returned by the request.
  * @param units
  *    The units of the request, or NULL if irrelevant.
  */
void report_uint64(const char *request, const char *name, uint64_t value,
    const char *units);

/**
  * The report function may be used to print a line of results for an
  * ioctl request.  Usually is more appropriate to use a type-specfic
  * variant, for consistent presentation.
  *
  * @param request
  *    The name of the ioctl request, suitable for copy-and-paste.
  * @param name
  *    The human readable English name of the result returned by the request.
  * @param value
  *    The value returned by the request.
  */
void report(const char *request, const char *name, const char *value);

/**
  * The report_semi_auto function is used to run an ioctl and print the
  * returned data.  All of the data allocation and de-allocation is
  * taken care of automagically.
  *
  * @param fildes
  *     The file descriptor to run the request against.
  * @param request
  *     The request to run.
  */
void report_semi_auto(int fildes, int request);

#endif /* IOCTL_SCAN_REPORT_H */
