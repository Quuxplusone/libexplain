dnl!
dnl! libexplain - Explain errno values returned by libc functions
dnl! Copyright (C) 2008-2014 Peter Miller
dnl! Written by Peter Miller <pmiller@opensource.org.au>
dnl!
dnl! This program is free software; you can redistribute it and/or
dnl! modify it under the terms of the GNU General Public License as
dnl! published by the Free Software Foundation; either version 3 of the
dnl! License, or (at your option) any later version.
dnl!
dnl! This program is distributed in the hope that it will be useful,
dnl! but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl! General Public License for more details.
dnl!
dnl! You should have received a copy of the GNU General Public License
dnl! along with this program. If not, see <http://www.gnu.org/licenses/>.
dnl!
AC_INIT(install-sh)
AC_CONFIG_HEADER(libexplain/config.h)
AC_USE_SYSTEM_EXTENSIONS
AC_LANG_C
AC_LANG_ASSERT(C)
AC_PROG_CC
AC_PROG_CPP
AC_CANONICAL_HOST
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_YACC

dnl!
dnl! See if the clang compiler is masquerading as gcc.
dnl!
dnl! Autoconf detects the Clang compiler as GNU GCC (clang sets defines
dnl! like __GNUC__ etc) but Clang is *not* completely compatible. If we
dnl! detect Clang we set ac_vc_c_compiler_gnu to 'no'.
dnl!
AC_TRY_LINK([
#include <stdio.h>
], [
#ifndef __clang__
This is not clang!
#endif
],
[cv_c_compiler_clang=yes
cv_c_compiler_gcc=no
],
[cv_c_compiler_clang=no]
)

case "$YACC" in
*bison*)
  ;;
*)
  AC_MSG_RESULT([
        You must have GNU Bison installed to build libexplain.
        Homepage: http://www.gnu.org/software/bison/])
  OK=no
  if apt-get --version > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo apt-get install bison
    ])
    OK=yes
  fi
  if yum --version > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo yum install bison
    ])
    OK=yes
  fi
  if pkg_info -a > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo pkg_add -r bison
    ])
    OK=yes
  fi
  if pkg --help > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo pkg install SUNWbison
    ])
    OK=yes
  fi
  if test "$OK" != "yes"; then
    AC_MSG_RESULT([
        If you are using a package based install, you will need the
        bison package.
    ])
  fi
  exit 1
  ;;
esac

AC_ISC_POSIX
AC_OBJEXT
AC_EXEEXT

dnl
dnl Large file support.
dnl
AC_FUNC_FSEEKO
AC_SYS_LARGEFILE

AC_TYPE_SIZE_T
AC_CHECK_SIZEOF([int])
AC_CHECK_SIZEOF([long])
AC_CHECK_SIZEOF([long long])
AC_CHECK_SIZEOF([off_t])
AC_CHECK_SIZEOF([ptrdiff_t])
AC_CHECK_SIZEOF([size_t])
AC_CHECK_SIZEOF([ssize_t])
AC_CHECK_SIZEOF([time_t])
AC_CHECK_SIZEOF([void *])


dnl @synopsis AC_ADD_CFLAGS
dnl
dnl Add the given option to CFLAGS, if it doesn't break the compiler
dnl
AC_DEFUN([AC_ADD_CFLAGS],
[AC_MSG_CHECKING([if $CC accepts $1])
  ac_add_cflags__old_cflags="$CFLAGS"
  CFLAGS="$CFLAGS $1"
  AC_TRY_LINK([#include <stdio.h>],
    [printf("Hello, World!\n"); return 0;],
    AC_MSG_RESULT([yes]),
    AC_MSG_RESULT([no])
    CFLAGS="$ac_add_cflags__old_cflags")
])

AC_ADD_CFLAGS(-Wall)
AC_ADD_CFLAGS(-Wextra)
AC_ADD_CFLAGS([-Wl,--as-needed])

dnl!
dnl! mostly for time parsing, we need floor()
dnl!
AC_CHECK_LIB(m, floor)

dnl!
dnl!  Solaris keeps socket functions in different library than libc.
dnl!
AC_CHECK_LIB(socket, bind, ,
    unset ac_cv_func_bind)

dnl
dnl we need to check for libiconv before we check for libintl
dnl
AC_CHECK_LIB(iconv, iconv, ,
    unset ac_cv_func_iconv)

dnl
dnl Solaris needs to use the GNU Gettext to have any chance of
dnl having working error message translation.  So we look for -lintl
dnl unconditionally.
dnl
AC_CHECK_LIB(intl, gettext, ,
  [
    unset ac_cv_func_gettext
    AC_CHECK_LIB(gettext, gettext, ,
      unset ac_cv_func_gettext)
  ]
)

dnl!
dnl!     On some BSD flavours, ftime() is defined in the -lcompat
dnl!     library.  The problem is, on some BSD flavours, the -lcompat
dnl!     library is not suitable for use by shared libraries, like
dnl!     libexplain.  So we leave it alone.
dnl!
dnl! AC_CHECK_LIB(compat, ftime)

dnl!
dnl! Access control support
dnl!
AC_CHECK_PROGS(LIBTOOL, libtool)

if test -z "$LIBTOOL"
then
  AC_MSG_RESULT([
        You must have GNU Libtool installed to build libexplain.
        Homepage: http://www.gnu.org/software/libtool/])
  OK=no
  if apt-get --version > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo apt-get install libtool
    ])
    OK=yes
  fi
  if yum --version > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo yum install libtool
    ])
    OK=yes
  fi
  if pkg_info -a > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo pkg_add -r libtool
    ])
    OK=yes
  fi
  if pkg --help > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo pkg install SUNWlibtool
    ])
    OK=yes
  fi
  if test "$OK" != "yes"; then
    AC_MSG_RESULT([
        If you are using a package based install, you will need the
        libtool package.
    ])
  fi
  exit 1
fi

dnl!
dnl!   libexplain doesn't *require* lsof, but it helps give better error
dnl!   messages.  However: Linux doesn't need lsof because /proc is
dnl!   actually useful, unlike BSD and Solaris.
dnl!
AC_CHECK_PROGS(LSOF, lsof)

if test -L /proc/self 2> /dev/null
then
    AC_DEFINE([PROC_SELF], [1],
    [Define this symbol if your system has symbolic link at /proc/self])
fi

if test -L /proc/$$/fd/0 2> /dev/null
then
    AC_DEFINE([PROC_PID_FD_N], [1],
    [Define this symbol if your system has symbolic links containing the
     absolute path of open files in /proc/nnn/fd/nnn])
fi

if test -L /proc/$$/path/0 2> /dev/null
then
    AC_DEFINE([PROC_PID_PATH_N], [1],
    [Define this symbol if your system has symbolic links containing the
     absolute path of open files in /proc/nnn/path/nnn])
fi

if test -L /proc/$$/path/cwd 2> /dev/null
then
    AC_DEFINE([PROC_PID_PATH_CWD], [1],
    [Define this symbol if your system has symbolic links containing the
    absolute path of the current directory of a process in /proc/pid/path/cwd])
fi

if test -L /proc/$$/cwd 2> /dev/null
then
    AC_DEFINE([PROC_PID_CWD], [1],
    [Define this symbol if your system has symbolic links containing the
    absolute path of the current directory of a process in /proc/pid/cwd])
fi

if test -L /proc/$$/path/root 2> /dev/null
then
    AC_DEFINE([PROC_PID_PATH_ROOT], [1],
    [Define this symbol if your system has symbolic links containing the
    absolute path of the root directory of a process in /proc/pid/path/root])
fi

if test -L /proc/$$/root 2> /dev/null
then
    AC_DEFINE([PROC_PID_ROOT], [1],
    [Define this symbol if your system has symbolic links containing the
    absolute path of the root directory of a process in /proc/pid/root])
fi

if test -L /proc/$$/path/a.out 2> /dev/null
then
    AC_DEFINE([PROC_PID_PATH_A_OUT], [1],
    [Define this symbol if your system has symbolic links containing the
    absolute path of the executable of a process in /proc/pid/path/a.out])
fi

if test -L /proc/$$/exe 2> /dev/null
then
    AC_DEFINE([PROC_PID_EXE], [1],
    [Define this symbol if your system has symbolic links containing the
    absolute path of the executable of a process in /proc/pid/exe])
fi

if test -f /proc/$$/cmdline 2> /dev/null
then
    AC_DEFINE([PROC_PID_CMDLINE], [1],
    [Define this symbol if your system has files containing the
    command line each process in /proc/pid/cmdline])
fi

AC_CHECK_PROGS(MSGFMT, gmsgfmt msgfmt)
AC_CHECK_PROGS(MSGCAT, gmsgcat msgcat)
AC_CHECK_PROGS(GROFF, groff roff)
AC_CHECK_PROGS(SOELIM, gsoelim soelim)
AC_CHECK_PROGS(REFER, refer grefer)

dnl
dnl We need some way to determine the value of the default MANPATH
dnl setting, so that we can append to it.  There are (at least) two
dnl incompatible man implementations out there.  The one used by Debian
dnl Linux has a "manpath -q" command, the one used by RedHat Linux has
dnl a "man -w" command.  If neither works, assume a constant string.
dnl
AC_CHECK_PROGS(MANPATH_PROG, manpath)

if test -n "$MANPATH_PROG"
then
    dnl
    dnl Sometimes manpath prints irritating warnings,
    dnl but not all versions have -q to silence them.
    dnl
    MANPATH_PROG="$MANPATH_PROG 2>/dev/null"
else
    dnl
    dnl Some versions of man have a -w option, but it's short for --whatis
    dnl and so they print nothing on stdout, and an error message about
    dnl the missing argument on stderr.  Some versions of man -w do the
    dnl same thing as manpath.
    dnl
    temp_str=`man -w 2>/dev/null`
    if test -n "$temp_str"
    then
        MANPATH_PROG="man -w 2>/dev/null"
    else
        MANPATH_PROG="echo /usr/man:/usr/share/man"
    fi
fi

AC_CHECK_PROGS(LOCALE_PROG, locale, , [${PATH}:/usr/sbin:/sbin])
if test -n "$LOCALE_PROG"
then
    AC_DEFINE(HAVE_LOCALE_PROG, 1,
    [Define this symbol if your system has the locale(1) program.])
fi

AC_CHECK_PROGS(LOCALE_GEN_PROG, locale-gen, , [${PATH}:/usr/sbin:/sbin])
if test -n "$LOCALE_GEN_PROG"
then
    AC_DEFINE(HAVE_LOCALE_GEN_PROG, 1,
    [Define this symbol if your system has the locale-gen(8) program.])
fi

dnl
dnl     Test if groff takes -ms or -mgs for the macro package.
dnl
AC_MSG_CHECKING([for $GROFF -ms macros])
if test `echo ' ' | groff -mgs 2> /dev/null | wc -l` -gt 0
then
    GROFF_MS_MACROS=gs
else
    GROFF_MS_MACROS=s
fi
AC_SUBST(GROFF_MS_MACROS)
AC_MSG_RESULT([-m$GROFF_MS_MACROS])

dnl
dnl     Test if groff takes -mm or -mgm for the macro package.
dnl
AC_MSG_CHECKING([for $GROFF -mm macros])
if test `echo ' ' | groff -mgm 2> /dev/null | wc -l` -gt 0
then
    GROFF_MM_MACROS=gm
else
    GROFF_MM_MACROS=m
fi
AC_SUBST(GROFF_MM_MACROS)
AC_MSG_RESULT([-m$GROFF_MM_MACROS])

AC_CHECK_HEADERS([         \
    _G_config.h            \
    acl/libacl.h           \
    arpa/inet.h            \
    bluetooth/bluetooth.h  \
    bluetooth/bnep.h       \
    bluetooth/cmtp.h       \
    bluetooth/hci.h        \
    bluetooth/hidp.h       \
    bluetooth/rfcomm.h     \
    dlfcn.h                \
    fcntl.h                \
    float.h                \
    ftw.h                  \
    getopt.h               \
    iconv.h                \
    inttypes.h             \
    libintl.h              \
    libio.h                \
    libv4l1-videodev.h     \
    limits.h               \
    linux/apm_bios.h       \
    linux/arcfb.h          \
    linux/ata.h            \
    linux/atm.h            \
    linux/atm_eni.h        \
    linux/atm_he.h         \
    linux/atm_idt77105.h   \
    linux/atm_nicstar.h    \
    linux/atm_tcp.h        \
    linux/atm_zatm.h       \
    linux/atmarp.h         \
    linux/atmclip.h        \
    linux/atmdev.h         \
    linux/atmlec.h         \
    linux/atmmpc.h         \
    linux/atmsvc.h         \
    linux/blkpg.h          \
    linux/blktrace_api.h   \
    linux/capi.h           \
    linux/cciss_ioctl.h    \
    linux/cdk.h            \
    linux/cdrom.h          \
    linux/chio.h           \
    linux/cm206.h          \
    linux/cm4000_cs.h      \
    linux/comstats.h       \
    linux/cyclades.h       \
    linux/dn.h             \
    linux/dvb/audio.h      \
    linux/dvb/ca.h         \
    linux/dvb/dmx.h        \
    linux/dvb/frontend.h   \
    linux/dvb/net.h        \
    linux/dvb/osd.h        \
    linux/dvb/video.h      \
    linux/ethtool.h        \
    linux/fb.h             \
    linux/fcntl.h          \
    linux/fd.h             \
    linux/fiemap.h         \
    linux/filter.h         \
    linux/firewire-cdev.h  \
    linux/fs.h             \
    linux/hayesesp.h       \
    linux/hdreg.h          \
    linux/hiddev.h         \
    linux/hidraw.h         \
    linux/hpet.h           \
    linux/i2o-dev.h        \
    linux/if.h             \
    linux/if_bridge.h      \
    linux/if_eql.h         \
    linux/if_tun.h         \
    linux/if_vlan.h        \
    linux/input.h          \
    linux/ipmi.h           \
    linux/ipv6.h           \
    linux/isdn.h           \
    linux/isdn_ppp.h       \
    linux/ivtvfb.h         \
    linux/joystick.h       \
    linux/kdev_t.h         \
    linux/kvm.h            \
    linux/lp.h             \
    linux/major.h          \
    linux/matroxfb.h       \
    linux/mii.h            \
    linux/mmtimer.h        \
    linux/msdos_fs.h       \
    linux/mtio.h           \
    linux/nbd.h            \
    linux/net_tstamp.h     \
    linux/nvram.h          \
    linux/phantom.h        \
    linux/pmu.h            \
    linux/ppdev.h          \
    linux/ppp-comp.h       \
    linux/radeonfb.h       \
    linux/random.h         \
    linux/raw.h            \
    linux/reiserfs_fs.h    \
    linux/rtc.h            \
    linux/serial.h         \
    linux/serio.h          \
    linux/smb_fs.h         \
    linux/socket.h         \
    linux/sockios.h        \
    linux/sonet.h          \
    linux/sonypi.h         \
    linux/spi/spidev.h     \
    linux/suspend_ioctls.h \
    linux/synclink.h       \
    linux/termios.h        \
    linux/tiocl.h          \
    linux/types.h          \
    linux/udf_fs_i.h       \
    linux/uinput.h         \
    linux/usb/g_printer.h  \
    linux/usb/gadgetfs.h   \
    linux/usbdevice_fs.h   \
    linux/video_decoder.h  \
    linux/video_encoder.h  \
    linux/videodev.h       \
    linux/videodev2.h      \
    linux/vt.h             \
    linux/watchdog.h       \
    locale.h               \
    mntent.h               \
    net/ethernet.h         \
    net/if_ppp.h           \
    net/ppp-comp.h         \
    net/ppp_defs.h         \
    netash/ash.h           \
    netax25/ax25.h         \
    netdb.h                \
    neteconet/ec.h         \
    netinet/in.h           \
    netipx/ipx.h           \
    netiucv/iucv.h         \
    netpacket/packet.h     \
    netrom/netrom.h        \
    poll.h                 \
    pwd.h                  \
    regex.h                \
    stddef.h               \
    stdint.h               \
    stdio_ext.h            \
    stdlib.h               \
    string.h               \
    sys/acl.h              \
    sys/capability.h       \
    sys/eventfd.h          \
    sys/file.h             \
    sys/filio.h            \
    sys/ioccom.h           \
    sys/ioctl.h            \
    sys/mman.h             \
    sys/mntent.h           \
    sys/mnttab.h           \
    sys/mount.h            \
    sys/mtio.h             \
    sys/param.h            \
    sys/ptrace.h           \
    sys/raw.h              \
    sys/resource.h         \
    sys/select.h           \
    sys/shm.h              \
    sys/signalfd.h         \
    sys/socket.h           \
    sys/sockio.h           \
    sys/stat.h             \
    sys/statfs.h           \
    sys/statvfs.h          \
    sys/sysmacros.h        \
    sys/termio.h           \
    sys/termios.h          \
    sys/time.h             \
    sys/timeb.h            \
    sys/timerfd.h          \
    sys/timex.h            \
    sys/ttycom.h           \
    sys/types.h            \
    sys/uio.h              \
    sys/user.h             \
    sys/ustat.h            \
    sys/utsname.h          \
    sys/un.h               \
    sys/vfs.h              \
    sys/wait.h             \
    syslog.h               \
    termio.h               \
    termios.h              \
    time.h                 \
    unistd.h               \
    ustat.h                \
    utime.h                \
    wchar.h                \
    wctype.h               \
    ])

dnl! =======================================================================
dnl!
dnl!  Headers that need special handling.
dnl!

AC_CHECK_HEADERS([linux/kd.h], [], [], [[
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#include <linux/kd.h>
#ifdef __FreeBSD__
=%fail^!
#endif
]])

AC_CHECK_HEADERS([net/if.h], [], [], [[
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
]])

AC_CHECK_HEADERS([net/if_arp.h], [], [], [[
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
]])

AC_CHECK_HEADERS([net/route.h], [], [], [[
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
]])

AC_CHECK_HEADERS([linux/atalk.h], [], [], [[
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
]])

AC_CHECK_HEADERS([linux/atmbr2684.h], [], [], [[
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
]])

AC_CHECK_HEADERS([linux/auto_fs.h], [], [], [[
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
]])

AC_CHECK_HEADERS([linux/auto_fs4.h], [], [], [[
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
]])

AC_CHECK_HEADERS([linux/ext2_fs.h], [], [], [[
#ifdef HAVE_LINUX_FS_H
#include <linux/fs.h>
#endif
]])

AC_CHECK_HEADERS([linux/if_bonding.h], [], [], [[
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
]])

AC_CHECK_HEADERS([linux/if_frad.h], [], [], [[
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
]])

AC_CHECK_HEADERS([linux/if_ppp.h], [], [], [[
#include <linux/types.h>
#include <linux/ppp_defs.h>
#include <linux/if.h>

#ifndef aligned_u64
#define aligned_u64 __u64 __attribute__((aligned(8)))
#endif

]])

AC_CHECK_HEADERS([linux/if_pppox.h], [], [], [[
#ifdef HAVE_NET_ETHERNET_H
#include <net/ethernet.h>
#endif
#ifdef HAVE_NET_IF_H
#include <net/if.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
]])

AC_CHECK_HEADERS([linux/irda.h], [], [], [[
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
]])

AC_CHECK_HEADERS([linux/ncp_fs.h], [], [], [[
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
]])

AC_CHECK_HEADERS([linux/netlink.h], [], [], [[
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
]])

AC_CHECK_HEADERS([linux/ppp_defs.h], [], [], [[
#include <linux/types.h>
]])

AC_CHECK_HEADERS([linux/x25.h], [], [], [[
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
]])

AC_CHECK_HEADERS([netrose/rose.h], [], [], [[
#ifdef HAVE_NETAX25_AX25_H
#include <netax25/ax25.h>
#endif
]])

dnl! this is from FreeBSD, and it's a right PITA
AC_CHECK_HEADERS([net/if_var.h], [], [], [[
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NET_IF_H
#include <net/if.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#include <net/if_var.h>
]])

dnl! this is from FreeBSD, and it's a right PITA
AC_CHECK_HEADERS([netinet/in_var.h], [], [], [[
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NET_IF_H
#include <net/if.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_NET_IF_VAR_H
#include <net/if_var.h>
#endif

#include <netinet/in_var.h>
]])

AC_CHECK_HEADERS([linux/scc.h], [], [], [[
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
]])

dnl! =======================================================================

dnl!
dnl!  We need to know whether getmntent takes one argument or two.
dnl!
AC_MSG_CHECKING([number of arguments to getmntent])
AC_TRY_COMPILE([
#ifdef HAVE_MNTENT_H
#include <mntent.h>
#endif
#ifdef HAVE_SYS_MNTENT_H
#include <sys/mntent.h>
#endif
#ifdef HAVE_SYS_MNTTAB_H
#include <sys/mnttab.h>
#endif
], [
    getmntent(0);
    return 0;
], [
    ac_av_getmntent_nargs=1
    AC_MSG_RESULT([1])
], [
    ac_av_getmntent_nargs=2
    AC_MSG_RESULT([2])
])
AC_DEFINE_UNQUOTED([GETMNTENT_NARGS], $ac_av_getmntent_nargs,
[Define this symbol to be the number of arguments required by the
getmntent function.])

dnl! =======================================================================

dnl!
dnl!  We need to know whether struct utsname::domainname exists.
dnl!
AC_MSG_CHECKING([for struct utsname::domainname])
AC_TRY_COMPILE([
#ifdef HAVE_SYS_UTSNAME_H
#include <sys/utsname.h>
#endif
], [
    struct utsname x;
    x.domainname[0] = 0;
], [
    AC_MSG_RESULT([yes])
    ac_av_utsname_domainname=1
], [
    AC_MSG_RESULT([no])
    ac_av_utsname_domainname=0
])
AC_DEFINE_UNQUOTED([HAVE_UTSNAME_DOMAINNAME], $ac_av_utsname_domainname,
[Define this symbol to indicate the presence of the utsname domainname
member.])

dnl! =======================================================================

dnl!
dnl!  We need to know whether va_list is compatible with const void *
dnl!
AC_MSG_CHECKING([what va_list is compatible with])
AC_TRY_COMPILE([
#include <stdarg.h>
], [
    va_list ap;
    const void *p;
    p = ap;
    return 0;
], [
    ac_av_va_list_void_star=1
    AC_MSG_RESULT(const void *)
], [
    ac_av_va_list_void_star=0
    AC_MSG_RESULT(nothing, sorry)
])
AC_DEFINE_UNQUOTED([VA_LIST_VOID_STAR], $ac_av_va_list_void_star,
[Define this symbol to 1 if va_list type is compatible with const void *.])

dnl! =======================================================================

dnl!
dnl!  We need to know whether statfs takes two arguments or four.
dnl!
AC_MSG_CHECKING([number of arguments to statfs])
AC_TRY_COMPILE([
#ifdef HAVE_SYS_MNTENT_H
#include <sys/statfs.h>
#endif
], [
    statfs(0, 0);
    return 0;
], [
    ac_av_statfs_nargs=2
    AC_MSG_RESULT([2])
], [
    ac_av_statfs_nargs=4
    AC_MSG_RESULT([4])
])
AC_DEFINE_UNQUOTED([STATFS_NARGS], $ac_av_statfs_nargs,
[Define to the number of arguments for the statfs function.])

dnl! =======================================================================

if test "$ac_cv_header_net_if_h" = yes -a "$ac_cv_header_linux_if_h" = yes
then
  # Can we include both of them at the same time?
  # Weirdly, the answer depends on the architecture.
  AC_MSG_CHECKING([if <net/if.h> and <linux/if.h> play nice])
  AC_TRY_COMPILE([
#include <sys/types.h>
#include <net/if.h>
#include <linux/if.h>],
    [return 0;],
    AC_MSG_RESULT([yes])
    AC_DEFINE(NET_IF_LINUX_IF_PLAY_NICE, 1,
        [Define this symbol if your system is able to include <net/if.h> and
        <linux/if.h> in the compilation unit.]),
    AC_MSG_RESULT([no]))
fi

if test "$HAVE_MNTENT_H" != "1"
then
    _PATH_MNTTAB=/dev/null
    test -f /etc/fstab && _PATH_MNTTAB=/etc/fstab
    test -f /etc/vfstab && _PATH_MNTTAB=/etc/vfstab # Solaris
    AC_DEFINE_UNQUOTED(_PATH_MNTTAB, ["${_PATH_MNTTAB}"],
    [File listing canonical interesting mount points.])

    _PATH_MOUNTED=${_PATH_MNTTAB}
    # OpenBSD doesn't have /etc/mtab
    test -f /etc/mtab && _PATH_MOUNTED=/etc/mtab
    test -f /etc/mnttab && _PATH_MOUNTED=/etc/mnttab
    AC_DEFINE_UNQUOTED(_PATH_MOUNTED, ["${_PATH_MOUNTED}"],
    [File listing currently active mount points.])
fi

AC_SEARCH_LIBS(cap_get_proc, cap)
AC_SEARCH_LIBS(dlopen, dl)
AC_SEARCH_LIBS(acl_free, acl)

AC_CHECK_TYPES([sigset_t, off_t, loff_t], , ,
[#include <signal.h>
#include <sys/types.h>])

AC_HEADER_DIRENT
AC_TYPE_SIGNAL
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_C_CONST

AC_C_INLINE
if test "$ac_cv_c_inline" != "no"
then
    AC_DEFINE(HAVE_INLINE,1,
    [Define this symbol if your C compiler has the inline keyword])
fi

AC_SYS_LONG_FILE_NAMES
AC_FUNC_STRERROR_R
AC_CHECK_FUNCS(     \
    __fpurge        \
    accept4         \
    acl_entries     \
    acl_from_text   \
    acl_get_fd      \
    acl_get_file    \
    acl_set_fd      \
    acl_set_file    \
    acl_to_any_text \
    acl_to_text     \
    adjtimex        \
    asprintf        \
    atexit          \
    cap_get_proc    \
    capget          \
    cfmakeraw       \
    chmod           \
    dirfd           \
    dladdr          \
    dlsym           \
    endgrent        \
    endmntent       \
    eventfd         \
    fchmod          \
    fchmodat        \
    fchown          \
    fchownat        \
    fdopendir       \
    flock           \
    fseeko          \
    fstatat         \
    fstatfs         \
    ftello          \
    ftime           \
    futimens        \
    futimes         \
    futimesat       \
    getdomainname   \
    getgrent        \
    getgrouplist    \
    gethostbyname   \
    gethostid       \
    gethostname     \
    getmntent       \
    getmntinfo      \
    getopt_long     \
    getpagesize     \
    getpriority     \
    getregid        \
    getresgid       \
    getresuid       \
    getreuid        \
    getrusage       \
    gettext         \
    hasmntopt       \
    iconv_close     \
    iconv_open      \
    lchmod          \
    lchown          \
    lchownat        \
    linkat          \
    lutime          \
    lutimeat        \
    lutimens        \
    lutimensat      \
    lutimes         \
    lutimesat       \
    mbrtowc         \
    mkdtemp         \
    mkostemp        \
    mmap            \
    mount           \
    munmap          \
    nanosleep       \
    on_exit         \
    openat          \
    pipe2           \
    poll            \
    ptrace          \
    putenv          \
    readv           \
    remove          \
    setdomainname   \
    setenv          \
    setgrent        \
    setlocale       \
    setmntent       \
    setpriority     \
    setregid        \
    setresgid       \
    setresuid       \
    setreuid        \
    setsid          \
    settimeofday    \
    shmat           \
    shmctl          \
    signalfd        \
    sleep           \
    snprintf        \
    statfs          \
    stime           \
    strcasecmp      \
    strcoll         \
    strerror        \
    strncasecmp     \
    strndup         \
    strnstr         \
    strsignal       \
    strtof          \
    strtold         \
    strverscmp      \
    tcdrain         \
    tcflow          \
    tcflush         \
    tcgetattr       \
    tcsendbreak     \
    tcsetattr       \
    timerfd_create  \
    uname           \
    unsetenv        \
    usleep          \
    ustat           \
    utimens         \
    utimensat       \
    utimes          \
    vasprintf       \
    vsnprintf       \
    wcwidth         \
    writev          \
    )

if test "$ac_cv_func_capget" = "yes"
then
  if test "$ac_cv_func_cap_get_proc" != "yes"
  then
    AC_MSG_RESULT([
        You must have libcap(1) installed to use libexplain.
Homepage: ftp://ftp.kernel.org/pub/linux/libs/security/linux-privs/kernel-2.2/])
    OK=no
    if apt-get --version > /dev/null 2> /dev/null; then
      AC_MSG_RESULT([
          The following command may be used to install it:
          sudo apt-get install libcap-dev
      ])
      OK=yes
    fi
    if yum --version > /dev/null 2> /dev/null; then
      AC_MSG_RESULT([
          The following command may be used to install it:
          sudo yum install libcap-devel
      ])
      OK=yes
    fi
    if test "$OK" != "yes"; then
      AC_MSG_RESULT([
          If you are using a package based install, you will need the
          libcap-dev or libcap-devel package, or similar.
      ])
    fi
    exit 1
  fi
fi

dnl!
dnl!    The mincore(2) function is broken more often than not.  The
dnl!    test_mincore program will sort out whether or not the mincore(2)
dnl!    function is at all useful.
dnl!
AC_MSG_CHECKING([for mincore])
AC_RUN_IFELSE([
#include "test_mincore/main.c"
], [
AC_MSG_RESULT([yes])
AC_DEFINE([HAVE_MINCORE], [1],
  [Define if mincore exists and also works for non-mmap memory.])
], [
AC_MSG_RESULT([broken])
])

dnl!
dnl!    Test to see if <stdio.h> defined off_t
dnl!
AC_MSG_CHECKING([stdio defines off_t])
AC_TRY_RUN([
#include <stdio.h>
int main(int argc, char **argv){
int x = sizeof(off_t);
return 0;
}]
, AC_DEFINE(HAVE_STDIO_OFF_T,1,
[Define this symbol if your system <stdio.h> defines the off_t type.])
AC_MSG_RESULT([yes]),
AC_MSG_RESULT([no]),
AC_MSG_RESULT(cross))

dnl!
dnl! Test to see if MS_MOVE is defineds and/or usable.
dnl! it may exist as an enum tag, but isn't defined to itself
dnl!
dnl!
AC_MSG_CHECKING([stdio defines off_t])
AC_TRY_RUN([
#include <sys/types.h>
#include <sys/mount.h>
int main(int argc, char **argv){
int x = MS_MOVE;
return 0;
}]
, AC_DEFINE(HAVE_SYS_MOUNT_MS_MOVE,1,
[Define this symbol if your system <sys/mount.h> defines MS_MOVE.])
AC_MSG_RESULT([yes]),
AC_MSG_RESULT([no]),
AC_MSG_RESULT(cross))



dnl!
dnl!    Test to see if <sys/types.h> defined off_t
dnl!
AC_MSG_CHECKING([sys/types.h defines off_t])
AC_TRY_RUN([
#include <sys/types.h>
int main(int argc, char **argv){
int x = sizeof(off_t);
return 0;
}]
, AC_DEFINE(HAVE_SYS_TYPES_OFF_T,1,
[Define this symbol if your system <sys/types.h> defines the off_t type.])
AC_MSG_RESULT([yes]),
AC_MSG_RESULT([no]),
AC_MSG_RESULT(cross))

dnl!
dnl!    Test to see if struct v4l2_hw_freq_seek has a member called "spacing".
dnl!
AC_MSG_CHECKING([struct v4l2_hw_freq_seek])
AC_TRY_RUN([
#include <linux/videodev2.h>
int main(argc, argv)int argc; char **argv;{
struct v4l2_hw_freq_seek x;
x.spacing = 0;
return 0;
}]
, AC_DEFINE(HAVE_v4l2_hw_freq_seek_spacing,1,
[Define this symbol if your system as <linux/videodev2.h> AND it
defines struct v4l2_hw_freq_seek to have a member called spacing.])
AC_MSG_RESULT([has spacing]),
AC_MSG_RESULT([does not have spacing]),
AC_MSG_RESULT(cross))

dnl!
dnl!    Test to see if V4L2_CTRL_TYPE_STRING is defined
dnl!    (we can't use #ifdef, it's an enum).
dnl!
AC_MSG_CHECKING([have V4L2_CTRL_TYPE_STRING])
AC_TRY_RUN([
#include <linux/videodev2.h>
int main(argc, argv)int argc; char **argv;{
int x = V4L2_CTRL_TYPE_STRING;
return 0;
}]
, AC_DEFINE(HAVE_V4L2_CTRL_TYPE_STRING,1,
[Define this symbol if your system as <linux/videodev2.h> AND it
defines enum member V4L2_CTRL_TYPE_STRING.])
AC_MSG_RESULT([yes]),
AC_MSG_RESULT([no]),
AC_MSG_RESULT(cross))

dnl!
dnl!    Test to see if V4L2_CTRL_TYPE_BITMASK is defined
dnl!    (we can't use #ifdef, it's an enum).
dnl!
AC_MSG_CHECKING([have V4L2_CTRL_TYPE_BITMASK])
AC_TRY_RUN([
#include <linux/videodev2.h>
int main(argc, argv)int argc; char **argv;{
int x = V4L2_CTRL_TYPE_BITMASK;
return 0;
}]
, AC_DEFINE(HAVE_V4L2_CTRL_TYPE_BITMASK,1,
[Define this symbol if your system as <linux/videodev2.h> AND it
defines enum member V4L2_CTRL_TYPE_BITMASK.])
AC_MSG_RESULT([yes]),
AC_MSG_RESULT([no]),
AC_MSG_RESULT(cross))

dnl!
dnl!    Test to see if struct v4l2_dbg_register defined
dnl!
AC_MSG_CHECKING([have struct v4l2_dbg_register])
AC_TRY_RUN([
#include <linux/videodev2.h>
int main(argc, argv)int argc; char **argv;{
struct v4l2_dbg_register x;
return 0;
}]
, AC_DEFINE(HAVE_V4L2_DBG_REGISTER,1,
[Define this symbol if your system as <linux/videodev2.h> AND it
defines struct v4l2_dbg_register.])
AC_MSG_RESULT([yes]),
AC_MSG_RESULT([no]),
AC_MSG_RESULT(cross))

dnl!
dnl!    Test to see if struct serial_rs485 is defined
dnl!
AC_MSG_CHECKING([have struct serial_rs485])
AC_TRY_RUN([
#include <linux/serial.h>
int main(argc, argv)int argc; char **argv;{
struct serial_rs485 x;
return 0;
}]
, AC_DEFINE(HAVE_STRUCT_SERIAL_RS485,1,
[Define this symbol if your system has <linux/serial.h> AND it
defines struct serial_rs485.])
AC_MSG_RESULT([yes]),
AC_MSG_RESULT([no]),
AC_MSG_RESULT(cross))

dnl!
dnl!    Test to see if struct serial_rs485 is defined
dnl!
AC_MSG_CHECKING([have struct serial_rs485.delay_rts_after_send])
AC_TRY_RUN([
#include <linux/serial.h>
int main(argc, argv)int argc; char **argv;{
struct serial_rs485 x;
x.delay_rts_after_send = 0;
return 0;
}]
, AC_DEFINE(HAVE_STRUCT_SERIAL_RS485_delay_rts_after_send,1,
[Define this symbol if your system has <linux/serial.h> AND it
defines struct serial_rs485.])
AC_MSG_RESULT([yes]),
AC_MSG_RESULT([no]),
AC_MSG_RESULT(cross))

dnl!
dnl!  Check that <linux/types.h> and <sys/types.h> can both be included
dnl!  together in the same compilation unit.
dnl!
AC_MSG_CHECKING([whether can include both <linux/types.h> and <sys/types.h>])
AC_TRY_RUN([
#include <sys/types.h>
#include <linux/types.h>
int main(argc, argv)int argc; char **argv;{
__u64 x;
return 0;
}]
, AC_DEFINE(CAN_INLUDE_BOTH_sys_types_h_AND_linux_types_h,1,
[Define this symbol if your system has <sys/types.h> and <linux/types.h> AND
they can both be included in the same compilation unit in that order.])
AC_MSG_RESULT([yes]),
AC_MSG_RESULT([no]),
AC_MSG_RESULT(cross))

dnl!
dnl! Does getpgrp need an argument?
dnl! BSD = yes
dnl! Posix = no
dnl!
AC_MSG_CHECKING([number of arguments to getpgrp])
AC_TRY_COMPILE([
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
], [
    getpgrp(0);
], [
    ac_av_getpgrp_nargs=1
    AC_MSG_RESULT([1])
], [
    ac_av_getpgrp_nargs=0
    AC_MSG_RESULT([0])
])
AC_DEFINE_UNQUOTED([GETPGRP_NARGS], $ac_av_getpgrp_nargs,
[Define this symbol to be the number of arguments required by the
getpgrp function.])

dnl!
dnl! Does setpgrp need two arguments?
dnl! BSD = yes
dnl! SysV = no
dnl!
AC_MSG_CHECKING([number of arguments to setpgrp])
AC_TRY_COMPILE([
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
], [
    setpgrp(0, 0);
], [
    ac_av_setpgrp_nargs=2
    AC_MSG_RESULT([1])
], [
    ac_av_setpgrp_nargs=0
    AC_MSG_RESULT([0])
])
AC_DEFINE_UNQUOTED([SETPGRP_NARGS], $ac_av_setpgrp_nargs,
[Define this symbol to be the number of arguments required by the
setpgrp function.])

dnl!
dnl!  Does <linux/types.h> define struct ustat?
dnl!  For example, Ubuntu Hardy has this bug.
dnl!
AC_MSG_CHECKING([whether <linux/types.h> defines struct ustat])
AC_TRY_RUN([
#include <linux/types.h>
int main(argc, argv)int argc; char **argv;{
struct ustat x;
x.f_fname[0] = 0;
return 0;
}]
, AC_DEFINE(LINUX_TYPES_H_STRUCT_USTAT,1,
[Define this symbol if your system has <linux/types.h> AND it
defines struct ustat.])
AC_MSG_RESULT([yes]),
AC_MSG_RESULT([no]),
AC_MSG_RESULT(cross))

dnl!
dnl! The strsignal function is usually defined in <string.h>
dnl! but is sometimes defined in <unistd.h>
dnl!
AC_CHECK_DECLS([strsignal,ustat,strverscmp],,, [
#ifndef __USE_GNU
#define __USE_GNU
#endif
#if HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_USTAT_H
#include <ustat.h>
#endif
])

dnl!
dnl! ptrace is just about the least architecture neutral syscall you
dnl! can imagine.  If something can be wrong with an API, then ptrace
dnl! works hard to get it wrong in the least architecture neutral way as
dnl! possible, and then some.
dnl!
AC_MSG_CHECKING([whether <sys/ptrace.h> defines struct user_regs_struct])
AC_TRY_RUN([
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_PTRACE_H
#include <sys/ptrace.h>
#endif
#ifdef HAVE_SYS_USER_H
#include <sys/user.h>
#endif
int main(argc, argv)int argc; char **argv;{
struct user_regs_struct x;
printf("%d\n", (int)sizeof(x));
return 0;
}]
, AC_DEFINE(SYS_PTRACE_USER_REGS_STRUCT,1,
[Define this symbol if your system has <sys/ptrace.h> or <sys/user.h> AND one
of them defines struct user_regs_struct.])
AC_MSG_RESULT([yes]),
AC_MSG_RESULT([no]),
AC_MSG_RESULT(cross))

AC_MSG_CHECKING([whether <sys/ptrace.h> defines struct user_fpregs_struct])
AC_TRY_RUN([
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_PTRACE_H
#include <sys/ptrace.h>
#endif
#ifdef HAVE_SYS_USER_H
#include <sys/user.h>
#endif
int main(argc, argv)int argc; char **argv;{
struct user_fpregs_struct x;
printf("%d\n", (int)sizeof(x));
return 0;
}]
, AC_DEFINE(SYS_PTRACE_USER_FPREGS_STRUCT,1,
[Define this symbol if your system has <sys/ptrace.h> or <sys/user.h> AND one
of them defines struct user_fpregs_struct.])
AC_MSG_RESULT([yes]),
AC_MSG_RESULT([no]),
AC_MSG_RESULT(cross))

AC_MSG_CHECKING([whether <sys/ptrace.h> defines struct user_fpxregs_struct])
AC_TRY_RUN([
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_PTRACE_H
#include <sys/ptrace.h>
#endif
#ifdef HAVE_SYS_USER_H
#include <sys/user.h>
#endif
int main(argc, argv)int argc; char **argv;{
struct user_fpxregs_struct x;
printf("%d\n", (int)sizeof(x));
return 0;
}]
, AC_DEFINE(SYS_PTRACE_USER_FPXREGS_STRUCT,1,
[Define this symbol if your system has <sys/ptrace.h> or <sys/user.h> AND one
of them defines struct user_fpxregs_struct.])
AC_MSG_RESULT([yes]),
AC_MSG_RESULT([no]),
AC_MSG_RESULT(cross))

AC_MSG_CHECKING([whether <sys/shm.h> defines struct ipc_perm::__key])
AC_TRY_RUN([
#include <sys/shm.h>
int main(argc, argv)int argc; char **argv;{
struct ipc_perm x;
printf("%d\n", x.__key);
return 0;
}]
, AC_DEFINE(SYS_SHM_H_struct_ipc_perm_underscore_key,1,
[Define this symbol if your system has <sys/shm.h> AND it
defines struct ipc_perm AND it has a __key member.])
AC_MSG_RESULT([yes]),
AC_MSG_RESULT([no]),
AC_MSG_RESULT(cross))

AC_MSG_CHECKING([whether <linux/videodev2.h> defines struct v4l2_buffer::input])
AC_TRY_RUN([
#include <linux/videodev2.h>
int main(argc, argv)int argc; char **argv;{
struct v4l2_buffer x;
printf("%d\n", (int)x.input);
return 0;
}]
, AC_DEFINE(LINUX_VIDEODEV2_H_struct_v4l2_buffer_input,1,
[Define this symbol if your system has <linux/videodev2.h> AND it
defines struct v4l2_buffer AND it has an 'input' member.])
AC_MSG_RESULT([yes]),
AC_MSG_RESULT([no]),
AC_MSG_RESULT(cross))

dnl!
dnl! Look for the NAME_MAX symbol.  This is the maximum length of a
dnl! pathname component (between th slashes) in a path.
dnl!
ac_av_name_max=0
AC_COMPUTE_INT([ac_av_name_max],
    [pathconf(\"/\", _PC_NAME_MAX)],
    [#include <unistd.h>])
if test "$ac_av_name_max" -gt 0
then
    AC_DEFINE_UNQUOTED(CONF_NAME_MAX, $ac_av_name_max,
        [Define this symbol to the value of pathconf("/", _PC_NAME_MAX) if your
        system does not have a native NAME_MAX symbol defined in the <limits.h>
        or <sys/param.h> system include files.])
fi

dnl! "

dnl!
dnl! aSomewhere around Linux kernel version 3.0.0 these symbols were
dnl! added.  The problem is they are members of an enum, not #defines.
dnl!
AC_MSG_CHECKING([whether V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE is defined])
AC_TRY_COMPILE([
#include <stdio.h>
#include </usr/include/linux/videodev2.h>
], [
#ifdef V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE
dont bother
#else
printf("%d\n", V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE);
#endif
]
, [AC_DEFINE(V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE,
V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE,
[Define this symbol as itself if your system has <linux/videodev2.h> and
it defines V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE as an enum member.])
AC_MSG_RESULT([yes])],
[AC_MSG_RESULT([no])]
)

AC_MSG_CHECKING([whether V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE is defined])
AC_TRY_COMPILE([
#include <stdio.h>
#include </usr/include/linux/videodev2.h>
], [
#ifdef V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE
dont bother
#else
printf("%d\n", V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE);
#endif
]
, [AC_DEFINE(V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE,
V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE,
[Define this symbol as itself if your system has <linux/videodev2.h> and
it defines V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE as an enum member.])
AC_MSG_RESULT([yes])],
[AC_MSG_RESULT([no])]
)

dnl!
dnl!    Do not do this until after all of the compiler and symbol tests
dnl!    are done, otherwise you get false negatives.
dnl!    Probably don't need to impose this on downstream.
dnl!
dnl! AC_ADD_CFLAGS(-Werror)

dnl
dnl   Test to find a Bourne shell which understands functions
dnl
AC_MSG_CHECKING([for a Bourne shell which understands functions])
if test "z$SH" = "z"; then
    if test -f /bin/sh5; then
        SH=/bin/sh5
    else
        SH=/bin/sh
    fi
fi
AC_SUBST(SH)
AC_DEFINE_UNQUOTED(CONF_SHELL, ["$SH"],
[Set this to be the absolute path of a Bourne shell
which understands functions.])
AC_MSG_RESULT($SH)

dnl
dnl Evaluate some of the variables, to remove ${prefix} references.
dnl This way, they can be used in C programs and Roff input.
dnl Make sure that libexplain is mentioned in the libdir and datadir paths;
dnl add it if it is not already there.
dnl
test "x$prefix" = xNONE && prefix="${ac_default_prefix-/usr/local}"
test "x$exec_prefix" = xNONE && exec_prefix="$prefix"
eval "bindir=$bindir"
eval "datarootdir=$datarootdir"
eval "includedir=$includedir"
eval "datadir=$datadir"
eval "libdir=$libdir"
eval "mandir=$mandir"
eval "sharedstatedir=$sharedstatedir"
case "$datadir" in
*/libexplain/* | */libexplain )
        ;;
*)
        datadir="${datadir}/libexplain"
        ;;
esac
case "$sharedstatedir" in
*/libexplain/* | */libexplain )
        ;;
*)
        sharedstatedir="${sharedstatedir}/libexplain"
        ;;
esac
eval "sysconfdir=$sysconfdir"

dnl support for NLSDIR option
AC_ARG_WITH(nlsdir,
        [  --with-nlsdir=PATH   specify where the locale stuff should go ])

if test "x$NLSDIR" = "x"; then
        if test "x$with_nlsdir" != "x"; then
                NLSDIR=$with_nlsdir
        else
                NLSDIR=${datarootdir}/locale
        fi
fi

AC_SUBST(NLSDIR)

dnl
dnl     If the gettext function exists, assume the msgfmt program does
dnl     also, and arrange for the Makefile to install the .mo files
dnl     from the .po files.
dnl
po_files=no
if test "$ac_cv_func_gettext" = "yes"
then
  if test -n "$MSGFMT"
  then
    po_files=yes
  fi
fi
if test "$po_files" = "no"
then
  AC_MSG_RESULT([
        Libexplain will emit much more useful error messages if
        you install GNU Gettext and then re-run this
        ./configure script.
        Remember to use \`\`rm config.cache'' before you do.
        Homepage: http://www.gnu.org/software/gettext/])
  OK=no
  if apt-get --version > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo apt-get install gettext
    ])
    OK=yes
  fi
  if yum --version > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo yum install gettext
    ])
    OK=yes
  fi
  if pkg_info -a > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo pkg_add -r gettext
    ])
    OK=yes
  fi
  if pkg --help > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo pkg install SUNWgnu-gettext
    ])
    OK=yes
  fi
  if test "$OK" != "yes"; then
    AC_MSG_RESULT([
        If you are using a package based install, you will need the
        gettext package.
    ])
  fi
fi
AC_SUBST(po_files)

dnl!
dnl!   libexplain doesn't *require* lsof,
dnl!   but it helps give better error messages.
dnl!
if test -z "$LSOF"
then
  AC_MSG_RESULT([
        Libexplain will emit much more useful error messages if
        you install lsof and then re-run this ./configure script.
        Remember to use \`\`rm config.cache'' before you do.
        Homepage: http://people.freebsd.org/~abe/
  ])
  OK=no
  if apt-get --version > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo apt-get install lsof
    ])
    OK=yes
  fi
  if yum --version > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo yum install lsof
    ])
    OK=yes
  fi
  if pkg_info -a > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo pkg_add -r lsof
    ])
    OK=yes
  fi
  if pkg --help > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo pkg install CSWlsof
    ])
    OK=yes
  fi
  if test "$OK" != "yes"; then
    AC_MSG_RESULT([
        If you are using a package based install, you will need the
        lsof package.
    ])
  fi
fi

dnl
dnl     If the soelim program exists, and understands the -I option,
dnl     arrange for the Makefile to install the .ps files from the
dnl     documentation source files.
dnl
if test -n "$SOELIM"
then
  if $SOELIM -I. /dev/null > /dev/null 2>&1
  then
    : nothing
  else
    AC_MSG_RESULT([
        The $SOELIM program does not understand the -I option.
        GNU Groff 1.15 or later works correctly.
        See the BUILDING file for details.])
    GROFF=
  fi
else
  GROFF=
fi
if test -n "$REFER"
then
    : nothing
else
    # It turns out that somehow Debian systems get a half-assed groff
    # install from build-essential.  Looking for "refer" is a better
    # test of the completeness of the groff install than either
    # "groff" or "soelim" on their own, plus it's needed for the build.
    GROFF=
fi
if test -n "$GROFF"
then
  HAVE_GROFF=yes
else
  HAVE_GROFF=no
  AC_MSG_RESULT([
        The libexplain documentation set and manual pages will
        be formatted and installed if you first install
        GNU Groff 1.15 or later and then re-run this ./configure script.
        Remember to use \`\`rm config.cache'' before you do.
        Homepage: http://www.gnu.org/software/groff/])
  OK=no
  if apt-get --version > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo apt-get install groff
    ])
    OK=yes
  fi
  if yum --version > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo yum install groff
    ])
    OK=yes
  fi
  if pkg --help > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo pkg install SUNWgroff
    ])
    OK=yes
  fi
  if test "$OK" != "yes"; then
    AC_MSG_RESULT([
        If you are using a package based install, you will need the
        groff package.
    ])
  fi
fi
AC_SUBST(HAVE_GROFF)

# program prefix is the bit to add to the start of the name
if test ${PROGRAM_PREFIX-NONE} != NONE -a ${program_prefix-NONE} != NONE; then
PROGRAM_PREFIX=${program_prefix}
fi
AC_SUBST(PROGRAM_PREFIX)

# program suffix is the bit to add to the end of the name (before .exe)
if test ${PROGRAM_SUFFIX-NONE} != NONE -a ${program_suffix-NONE} != NONE; then
PROGRAM_SUFFIX=${program_suffix}
fi
AC_SUBST(PROGRAM_SUFFIX)

AH_TOP(
[/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2014 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBEXPLAIN_CONFIG_H
#define LIBEXPLAIN_CONFIG_H
])

AH_BOTTOM([
/*
 * There is more to do, but we need to insulate it from config.status,
 * because it screws up the #undef lines.  They are all implications of
 * the above information, so there is no need for you to edit the file,
 * if you are configuring libexplain manually.
 */
#include <libexplain/config.messy.h>

#endif /* LIBEXPLAIN_CONFIG_H */])

AC_OUTPUT(Makefile)
dnl! vim: set ts=8 sw=4 et :
