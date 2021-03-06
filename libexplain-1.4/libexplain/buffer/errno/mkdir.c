/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2010, 2013 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/assert.h>
#include <libexplain/ac/errno.h>
#include <libexplain/ac/sys/stat.h>
#include <libexplain/ac/unistd.h>

#include <libexplain/buffer/eacces.h>
#include <libexplain/buffer/eexist.h>
#include <libexplain/buffer/efault.h>
#include <libexplain/buffer/eloop.h>
#include <libexplain/buffer/emlink.h>
#include <libexplain/buffer/enametoolong.h>
#include <libexplain/buffer/enoent.h>
#include <libexplain/buffer/enomem.h>
#include <libexplain/buffer/enospc.h>
#include <libexplain/buffer/enotdir.h>
#include <libexplain/buffer/eperm.h>
#include <libexplain/buffer/erofs.h>
#include <libexplain/buffer/errno/generic.h>
#include <libexplain/buffer/errno/mkdir.h>
#include <libexplain/buffer/errno/path_resolution.h>
#include <libexplain/buffer/mount_point.h>
#include <libexplain/buffer/permission_mode.h>
#include <libexplain/buffer/pointer.h>
#include <libexplain/dirname.h>
#include <libexplain/get_link_max.h>
#include <libexplain/explanation.h>


static void
explain_buffer_errno_mkdir_system_call(explain_string_buffer_t *sb,
    int errnum, const char *pathname, int mode)
{
    explain_string_buffer_puts(sb, "mkdir(pathname = ");
    if (errnum == EFAULT)
        explain_buffer_pointer(sb, pathname);
    else
        explain_string_buffer_puts_quoted(sb, pathname);
    explain_string_buffer_puts(sb, ", mode = ");
    explain_buffer_permission_mode(sb, mode);
    explain_string_buffer_putc(sb, ')');
}


void
explain_buffer_errno_mkdir_explanation(explain_string_buffer_t *sb,
    int errnum, const char *syscall_name, const char *pathname, int mode)
{
    explain_final_t final_component;

    explain_final_init(&final_component);
    final_component.must_exist = 0;
    final_component.must_not_exist = 1;
    final_component.want_to_create = 1;
    final_component.follow_symlink = 0;
    final_component.st_mode = S_IFDIR | mode;

    switch (errnum)
    {
    case EACCES:
        explain_buffer_eacces(sb, pathname, "pathname", &final_component);
        break;

    case EEXIST:
        if
        (
            explain_buffer_errno_path_resolution
            (
                sb,
                errnum,
                pathname,
                "pathname",
                &final_component
            )
        )
        {
            explain_buffer_eexist(sb, pathname);
        }
        break;

    case EFAULT:
        explain_buffer_efault(sb, "pathname");
        break;

    case EMLINK:
        {
            long            link_max;
            struct stat     parent_st;
            char            parent[PATH_MAX];

            /*
             * The containing directory may already have too many links.
             */
            explain_dirname(parent, pathname, sizeof(parent));
            link_max = explain_get_link_max(parent);
            assert(link_max > 0);
            if
            (
                stat(parent, &parent_st) >= 0
            &&
                parent_st.st_nlink >= (unsigned long)link_max
            )
            {
                explain_buffer_emlink_mkdir(sb, parent, "pathname");
                break;
            }
        }

        /*
         * On BSD an EMLINK error can be returned where Linux would
         * return ELOOP.  So fall through...
         */

    case ELOOP:
        explain_buffer_eloop(sb, pathname, "pathname", &final_component);
        break;

    case ENAMETOOLONG:
        explain_buffer_enametoolong
        (
            sb,
            pathname,
            "pathname",
            &final_component
        );
        break;

    case ENOENT:
        explain_buffer_enoent(sb, pathname, "pathname", &final_component);
        break;

    case ENOMEM:
        explain_buffer_enomem_kernel(sb);
        break;

    case ENOSPC:
        explain_buffer_enospc(sb, pathname, "pathname");
        break;

    case ENOTDIR:
        explain_buffer_enotdir(sb, pathname, "pathname", &final_component);
        break;

    case EPERM:
        explain_buffer_eperm_mknod(sb, pathname, "pathname", S_IFDIR);
        break;

    case EROFS:
        explain_buffer_erofs(sb, pathname, "pathname");
        break;

    default:
        explain_buffer_errno_generic(sb, errnum, syscall_name);
        break;
    }
}


void
explain_buffer_errno_mkdir(explain_string_buffer_t *sb, int errnum,
    const char *pathname, int mode)
{
    explain_explanation_t exp;

    explain_explanation_init(&exp, errnum);
    explain_buffer_errno_mkdir_system_call
    (
        &exp.system_call_sb,
        errnum,
        pathname,
        mode
    );
    explain_buffer_errno_mkdir_explanation
    (
        &exp.explanation_sb,
        errnum,
        "mkdir",
        pathname,
        mode
    );
    explain_explanation_assemble(&exp, sb);
}


/* vim: set ts=8 sw=4 et : */
