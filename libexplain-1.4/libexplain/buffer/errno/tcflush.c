/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2013 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but
 * WITHOUT ANY WARRANTY; without even the implied warranty
 * ofMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNULesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/errno.h>

#include <libexplain/buffer/ebadf.h>
#include <libexplain/buffer/einval.h>
#include <libexplain/buffer/enosys.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/tcflush.h>
#include <libexplain/buffer/fildes.h>
#include <libexplain/buffer/tcflush_selector.h>
#include <libexplain/explanation.h>


static void
explain_buffer_errno_tcflush_system_call(explain_string_buffer_t *sb, int
    errnum, int fildes, int selector)
{
    (void)errnum;
    explain_string_buffer_puts(sb, "tcflush(fildes = ");
    explain_buffer_fildes(sb, fildes);
    explain_string_buffer_puts(sb, ", selector = ");
    explain_buffer_tcflush_selector(sb, selector);
    explain_string_buffer_putc(sb, ')');
}


void
explain_buffer_errno_tcflush_explanation(explain_string_buffer_t *sb, int
    errnum, const char *syscall_name, int fildes, int selector)
{
    /*
     * http://www.opengroup.org/onlinepubs/009695399/functions/tcflush.html
     */
    (void)selector;
    switch (errnum)
    {
    case EBADF:
        explain_buffer_ebadf(sb, fildes, "fildes");
        break;

    case EINVAL:
        explain_buffer_einval_vague(sb, "selector");
        break;

    case ENOTTY:
    case ENOSYS:
#if defined(EOPNOTSUPP) && EOPNOTSUPP != ENOSYS
    case EOPNOTSUPP:
#endif
#ifdef ENOIOCTLCMD
    case ENOIOCTLCMD:
#endif
#ifdef ENOIOCTL
    case ENONOIOCTL:
#endif
        explain_buffer_enosys_fildes(sb, fildes, "fildes", syscall_name);
        break;

    default:
        explain_buffer_errno_generic(sb, errnum, syscall_name);
        break;
    }
}


void
explain_buffer_errno_tcflush(explain_string_buffer_t *sb, int errnum, int
    fildes, int selector)
{
    explain_explanation_t exp;

    explain_explanation_init(&exp, errnum);
    explain_buffer_errno_tcflush_system_call(&exp.system_call_sb, errnum,
        fildes, selector);
    explain_buffer_errno_tcflush_explanation(&exp.explanation_sb, errnum,
        "tcflush", fildes, selector);
    explain_explanation_assemble(&exp, sb);
}


/* vim: set ts=8 sw=4 et : */
