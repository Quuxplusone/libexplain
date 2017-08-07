/*
 * libexplain - a library of system-call-specific strerror replacements
 * Copyright (C) 2011, 2013 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>
#include <libexplain/ac/sys/ioctl.h>
#include <libexplain/ac/termios.h>

#include <libexplain/buffer/int.h>
#include <libexplain/buffer/is_the_null_pointer.h>
#include <libexplain/iocontrol/generic.h>
#include <libexplain/iocontrol/tiocgdev.h>

#ifdef TIOCGDEV


const explain_iocontrol_t explain_iocontrol_tiocgdev =
{
    "TIOCGDEV", /* name */
    TIOCGDEV, /* value */
    0, /* disambiguate */
    0, /* print_name */
    explain_iocontrol_generic_print_data_pointer, /* print_data */
    0, /* print_explanation */
    explain_iocontrol_generic_print_data_uint_star, /* print_data_returned */
    sizeof(unsigned int), /* data_size */
    "unsigned int *", /* data_type */
    0, /* flags */
    __FILE__,
    __LINE__,
};

#else /* ndef TIOCGDEV */

const explain_iocontrol_t explain_iocontrol_tiocgdev =
{
    0, /* name */
    0, /* value */
    0, /* disambiguate */
    0, /* print_name */
    0, /* print_data */
    0, /* print_explanation */
    0, /* print_data_returned */
    0, /* data_size */
    0, /* data_type */
    0, /* flags */
    __FILE__,
    __LINE__,
};

#endif /* TIOCGDEV */

/* vim: set ts=8 sw=4 et : */