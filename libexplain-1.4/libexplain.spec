#
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#
#    W   W    A    RRRR   N   N   III  N   N  III  N   N   GGG
#    W   W   A A   R   R  NN  N    I   NN  N   I   NN  N  G   G
#    W   W  A   A  RRRR   N N N    I   N N N   I   N N N  G
#    W W W  AAAAA  R R    N  NN    I   N  NN   I   N  NN  G  GG
#    W W W  A   A  R  R   N   N    I   N   N   I   N   N  G   G
#     W W   A   A  R   R  N   N   III  N   N  III  N   N   GGG
#
# Warning: DO NOT send patches which fix this file. IT IS NOT the original
# source file. This file is GENERATED from the Aegis repository file manifest.
# If you find a bug in this file, it could well be an Aegis bug.
#
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#
Summary: library of syscall-specific strerror replacements
Name: libexplain
Version: 1.4.D001
Release: 1
License: LGPL, GPL
Group: Development/Tools
Source: http://libexplain.sourceforge.net/%{name}-%{version}.tar.gz
URL: http://libexplain.sourceforge.net/
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
BuildPrereq: bison, diffutils, ghostscript, glibc-devel, groff, libcap-devel
BuildPrereq: libtool, lsof, sharutils

%description
This package provides a library which may be used to explain Unix and Linux
system call errors. The library is not quite a drop-in replacement for strerror,
but it comes close, with each system call having a dedicated libexplain
function.

%package bin
Summary: libexplain programs

%description bin
library of syscall-specific strerror replacements

%package libs51
Summary: libexplain libraries
Group: Development/Tools

%description libs51
This package provides a library which may be used to explain Unix and Linux
system call errors. The library is not quite a drop-in replacement for strerror,
but it comes close, with each system call having a dedicated libexplain
function.

%package devel
Summary: libexplain development files
Group: Development/Tools
Requires: libexplain-libs%{?_isa} = %{?epoch:%{epoch}:}%{version}-%{release}

%description devel
This package contains static libraries and header files for compiling
applications that use libexplain to explain Unix and Linux system call errors.

%post
/sbin/ldconfig

%postun
/sbin/ldconfig


%prep
%setup -q


%build
%configure --sysconfdir=/etc --prefix=%{_prefix} --mandir=%{_mandir} \
		--with-nlsdir=%{_datadir}/locale
make


%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install
rm -f $RPM_BUILD_ROOT/usr/lib/*.la


%check
make sure


%clean
rm -rf $RPM_BUILD_ROOT


%files bin
%defattr (-,root,root,-)
%doc LICENSE BUILDING README
%doc %{_datarootdir}/doc/libexplain/building.pdf
%doc %{_datarootdir}/doc/libexplain/new_system_call.pdf
%doc %{_datarootdir}/doc/libexplain/readme.pdf
%doc %{_datarootdir}/doc/libexplain/reference.pdf
%{_bindir}/explain
%{_mandir}/man1/explain.1
%{_mandir}/man1/explain_lca2010.1
%{_mandir}/man1/explain_license.1


%files libs
%defattr (-,root,root,-)
%dir %{_localedir}/de
%dir %{_localedir}/de/LC_MESSAGES
%{_localedir}/de/LC_MESSAGES/libexplain.mo
%{_libdir}/libexplain.so.*


%files devel
%defattr (-,root,root,-)
%{_includedir}/libexplain/accept.h
%{_includedir}/libexplain/accept4.h
%{_includedir}/libexplain/access.h
%{_includedir}/libexplain/acct.h
%{_includedir}/libexplain/acl_from_text.h
%{_includedir}/libexplain/acl_get_fd.h
%{_includedir}/libexplain/acl_get_file.h
%{_includedir}/libexplain/acl_set_fd.h
%{_includedir}/libexplain/acl_set_file.h
%{_includedir}/libexplain/acl_to_text.h
%{_includedir}/libexplain/adjtime.h
%{_includedir}/libexplain/adjtimex.h
%{_includedir}/libexplain/asprintf.h
%{_includedir}/libexplain/bind.h
%{_includedir}/libexplain/calloc.h
%{_includedir}/libexplain/chdir.h
%{_includedir}/libexplain/chmod.h
%{_includedir}/libexplain/chown.h
%{_includedir}/libexplain/chroot.h
%{_includedir}/libexplain/close.h
%{_includedir}/libexplain/closedir.h
%{_includedir}/libexplain/config.messy.h
%{_includedir}/libexplain/connect.h
%{_includedir}/libexplain/creat.h
%{_includedir}/libexplain/dirfd.h
%{_includedir}/libexplain/dup.h
%{_includedir}/libexplain/dup2.h
%{_includedir}/libexplain/endgrent.h
%{_includedir}/libexplain/eventfd.h
%{_includedir}/libexplain/execlp.h
%{_includedir}/libexplain/execv.h
%{_includedir}/libexplain/execve.h
%{_includedir}/libexplain/execvp.h
%{_includedir}/libexplain/exit.h
%{_includedir}/libexplain/fchdir.h
%{_includedir}/libexplain/fchmod.h
%{_includedir}/libexplain/fchown.h
%{_includedir}/libexplain/fchownat.h
%{_includedir}/libexplain/fclose.h
%{_includedir}/libexplain/fcntl.h
%{_includedir}/libexplain/fdopen.h
%{_includedir}/libexplain/fdopendir.h
%{_includedir}/libexplain/feof.h
%{_includedir}/libexplain/ferror.h
%{_includedir}/libexplain/fflush.h
%{_includedir}/libexplain/fgetc.h
%{_includedir}/libexplain/fgetpos.h
%{_includedir}/libexplain/fgets.h
%{_includedir}/libexplain/filename.h
%{_includedir}/libexplain/fileno.h
%{_includedir}/libexplain/flock.h
%{_includedir}/libexplain/fopen.h
%{_includedir}/libexplain/fork.h
%{_includedir}/libexplain/fpathconf.h
%{_includedir}/libexplain/fprintf.h
%{_includedir}/libexplain/fpurge.h
%{_includedir}/libexplain/fputc.h
%{_includedir}/libexplain/fputs.h
%{_includedir}/libexplain/fread.h
%{_includedir}/libexplain/freopen.h
%{_includedir}/libexplain/fseek.h
%{_includedir}/libexplain/fseeko.h
%{_includedir}/libexplain/fsetpos.h
%{_includedir}/libexplain/fstat.h
%{_includedir}/libexplain/fstatat.h
%{_includedir}/libexplain/fstatfs.h
%{_includedir}/libexplain/fstatvfs.h
%{_includedir}/libexplain/fsync.h
%{_includedir}/libexplain/ftell.h
%{_includedir}/libexplain/ftello.h
%{_includedir}/libexplain/ftime.h
%{_includedir}/libexplain/ftruncate.h
%{_includedir}/libexplain/futimens.h
%{_includedir}/libexplain/futimes.h
%{_includedir}/libexplain/futimesat.h
%{_includedir}/libexplain/fwrite.h
%{_includedir}/libexplain/gcc_attributes.h
%{_includedir}/libexplain/getaddrinfo.h
%{_includedir}/libexplain/getc.h
%{_includedir}/libexplain/getchar.h
%{_includedir}/libexplain/getcwd.h
%{_includedir}/libexplain/getdomainname.h
%{_includedir}/libexplain/getgrent.h
%{_includedir}/libexplain/getgrouplist.h
%{_includedir}/libexplain/getgroups.h
%{_includedir}/libexplain/gethostbyname.h
%{_includedir}/libexplain/gethostid.h
%{_includedir}/libexplain/gethostname.h
%{_includedir}/libexplain/getpeername.h
%{_includedir}/libexplain/getpgid.h
%{_includedir}/libexplain/getpgrp.h
%{_includedir}/libexplain/getpriority.h
%{_includedir}/libexplain/getresgid.h
%{_includedir}/libexplain/getresuid.h
%{_includedir}/libexplain/getrlimit.h
%{_includedir}/libexplain/getrusage.h
%{_includedir}/libexplain/getsockname.h
%{_includedir}/libexplain/getsockopt.h
%{_includedir}/libexplain/gettimeofday.h
%{_includedir}/libexplain/getw.h
%{_includedir}/libexplain/iconv.h
%{_includedir}/libexplain/iconv_close.h
%{_includedir}/libexplain/iconv_open.h
%{_includedir}/libexplain/ioctl.h
%{_includedir}/libexplain/kill.h
%{_includedir}/libexplain/large_file_support.h
%{_includedir}/libexplain/lchmod.h
%{_includedir}/libexplain/lchown.h
%{_includedir}/libexplain/lchownat.h
%{_includedir}/libexplain/libexplain.h
%{_includedir}/libexplain/link.h
%{_includedir}/libexplain/linkat.h
%{_includedir}/libexplain/listen.h
%{_includedir}/libexplain/lseek.h
%{_includedir}/libexplain/lstat.h
%{_includedir}/libexplain/lutimes.h
%{_includedir}/libexplain/malloc.h
%{_includedir}/libexplain/mkdir.h
%{_includedir}/libexplain/mkdtemp.h
%{_includedir}/libexplain/mknod.h
%{_includedir}/libexplain/mkostemp.h
%{_includedir}/libexplain/mkstemp.h
%{_includedir}/libexplain/mktemp.h
%{_includedir}/libexplain/mmap.h
%{_includedir}/libexplain/mount.h
%{_includedir}/libexplain/munmap.h
%{_includedir}/libexplain/nanosleep.h
%{_includedir}/libexplain/nice.h
%{_includedir}/libexplain/open.h
%{_includedir}/libexplain/openat.h
%{_includedir}/libexplain/opendir.h
%{_includedir}/libexplain/output.h
%{_includedir}/libexplain/pathconf.h
%{_includedir}/libexplain/pclose.h
%{_includedir}/libexplain/pipe.h
%{_includedir}/libexplain/pipe2.h
%{_includedir}/libexplain/poll.h
%{_includedir}/libexplain/popen.h
%{_includedir}/libexplain/pread.h
%{_includedir}/libexplain/printf.h
%{_includedir}/libexplain/program_name.h
%{_includedir}/libexplain/ptrace.h
%{_includedir}/libexplain/public_config.h
%{_includedir}/libexplain/putc.h
%{_includedir}/libexplain/putchar.h
%{_includedir}/libexplain/putenv.h
%{_includedir}/libexplain/puts.h
%{_includedir}/libexplain/putw.h
%{_includedir}/libexplain/pwrite.h
%{_includedir}/libexplain/raise.h
%{_includedir}/libexplain/read.h
%{_includedir}/libexplain/readdir.h
%{_includedir}/libexplain/readlink.h
%{_includedir}/libexplain/readv.h
%{_includedir}/libexplain/realloc.h
%{_includedir}/libexplain/realpath.h
%{_includedir}/libexplain/remove.h
%{_includedir}/libexplain/rename.h
%{_includedir}/libexplain/rmdir.h
%{_includedir}/libexplain/select.h
%{_includedir}/libexplain/setbuf.h
%{_includedir}/libexplain/setbuffer.h
%{_includedir}/libexplain/setdomainname.h
%{_includedir}/libexplain/setenv.h
%{_includedir}/libexplain/setgid.h
%{_includedir}/libexplain/setgrent.h
%{_includedir}/libexplain/setgroups.h
%{_includedir}/libexplain/sethostname.h
%{_includedir}/libexplain/setlinebuf.h
%{_includedir}/libexplain/setpgid.h
%{_includedir}/libexplain/setpgrp.h
%{_includedir}/libexplain/setpriority.h
%{_includedir}/libexplain/setregid.h
%{_includedir}/libexplain/setresgid.h
%{_includedir}/libexplain/setresuid.h
%{_includedir}/libexplain/setreuid.h
%{_includedir}/libexplain/setsid.h
%{_includedir}/libexplain/setsockopt.h
%{_includedir}/libexplain/settimeofday.h
%{_includedir}/libexplain/setuid.h
%{_includedir}/libexplain/setvbuf.h
%{_includedir}/libexplain/shmat.h
%{_includedir}/libexplain/shmctl.h
%{_includedir}/libexplain/signalfd.h
%{_includedir}/libexplain/sleep.h
%{_includedir}/libexplain/snprintf.h
%{_includedir}/libexplain/socket.h
%{_includedir}/libexplain/socketpair.h
%{_includedir}/libexplain/sprintf.h
%{_includedir}/libexplain/stat.h
%{_includedir}/libexplain/statfs.h
%{_includedir}/libexplain/statvfs.h
%{_includedir}/libexplain/stime.h
%{_includedir}/libexplain/strcoll.h
%{_includedir}/libexplain/strdup.h
%{_includedir}/libexplain/strndup.h
%{_includedir}/libexplain/strtod.h
%{_includedir}/libexplain/strtof.h
%{_includedir}/libexplain/strtol.h
%{_includedir}/libexplain/strtold.h
%{_includedir}/libexplain/strtoll.h
%{_includedir}/libexplain/strtoul.h
%{_includedir}/libexplain/strtoull.h
%{_includedir}/libexplain/symlink.h
%{_includedir}/libexplain/system.h
%{_includedir}/libexplain/tcdrain.h
%{_includedir}/libexplain/tcflow.h
%{_includedir}/libexplain/tcflush.h
%{_includedir}/libexplain/tcgetattr.h
%{_includedir}/libexplain/tcsendbreak.h
%{_includedir}/libexplain/tcsetattr.h
%{_includedir}/libexplain/telldir.h
%{_includedir}/libexplain/tempnam.h
%{_includedir}/libexplain/time.h
%{_includedir}/libexplain/timerfd_create.h
%{_includedir}/libexplain/tmpfile.h
%{_includedir}/libexplain/tmpnam.h
%{_includedir}/libexplain/truncate.h
%{_includedir}/libexplain/uname.h
%{_includedir}/libexplain/ungetc.h
%{_includedir}/libexplain/unlink.h
%{_includedir}/libexplain/unsetenv.h
%{_includedir}/libexplain/usleep.h
%{_includedir}/libexplain/ustat.h
%{_includedir}/libexplain/utime.h
%{_includedir}/libexplain/utimens.h
%{_includedir}/libexplain/utimensat.h
%{_includedir}/libexplain/utimes.h
%{_includedir}/libexplain/vasprintf.h
%{_includedir}/libexplain/vfork.h
%{_includedir}/libexplain/vfprintf.h
%{_includedir}/libexplain/vprintf.h
%{_includedir}/libexplain/vsnprintf.h
%{_includedir}/libexplain/vsprintf.h
%{_includedir}/libexplain/wait.h
%{_includedir}/libexplain/wait3.h
%{_includedir}/libexplain/wait4.h
%{_includedir}/libexplain/waitpid.h
%{_includedir}/libexplain/write.h
%{_includedir}/libexplain/writev.h
%{_libdir}/pkgconfig/libexplain.pc
%{_mandir}/man3/explain.3
%{_mandir}/man3/explain_accept.3
%{_mandir}/man3/explain_accept4.3
%{_mandir}/man3/explain_accept4_or_die.3
%{_mandir}/man3/explain_accept_or_die.3
%{_mandir}/man3/explain_access.3
%{_mandir}/man3/explain_access_or_die.3
%{_mandir}/man3/explain_acct.3
%{_mandir}/man3/explain_acct_or_die.3
%{_mandir}/man3/explain_acl_from_text.3
%{_mandir}/man3/explain_acl_from_text_or_die.3
%{_mandir}/man3/explain_acl_get_fd.3
%{_mandir}/man3/explain_acl_get_fd_or_die.3
%{_mandir}/man3/explain_acl_get_file.3
%{_mandir}/man3/explain_acl_get_file_or_die.3
%{_mandir}/man3/explain_acl_set_fd.3
%{_mandir}/man3/explain_acl_set_fd_or_die.3
%{_mandir}/man3/explain_acl_set_file.3
%{_mandir}/man3/explain_acl_set_file_or_die.3
%{_mandir}/man3/explain_acl_to_text.3
%{_mandir}/man3/explain_acl_to_text_or_die.3
%{_mandir}/man3/explain_adjtime.3
%{_mandir}/man3/explain_adjtime_or_die.3
%{_mandir}/man3/explain_adjtimex.3
%{_mandir}/man3/explain_adjtimex_or_die.3
%{_mandir}/man3/explain_asprintf.3
%{_mandir}/man3/explain_asprintf_or_die.3
%{_mandir}/man3/explain_bind.3
%{_mandir}/man3/explain_bind_or_die.3
%{_mandir}/man3/explain_calloc.3
%{_mandir}/man3/explain_calloc_or_die.3
%{_mandir}/man3/explain_chdir.3
%{_mandir}/man3/explain_chdir_or_die.3
%{_mandir}/man3/explain_chmod.3
%{_mandir}/man3/explain_chmod_or_die.3
%{_mandir}/man3/explain_chown.3
%{_mandir}/man3/explain_chown_or_die.3
%{_mandir}/man3/explain_chroot.3
%{_mandir}/man3/explain_chroot_or_die.3
%{_mandir}/man3/explain_close.3
%{_mandir}/man3/explain_close_or_die.3
%{_mandir}/man3/explain_closedir.3
%{_mandir}/man3/explain_closedir_or_die.3
%{_mandir}/man3/explain_connect.3
%{_mandir}/man3/explain_connect_or_die.3
%{_mandir}/man3/explain_creat.3
%{_mandir}/man3/explain_creat_or_die.3
%{_mandir}/man3/explain_dirfd.3
%{_mandir}/man3/explain_dirfd_or_die.3
%{_mandir}/man3/explain_dup.3
%{_mandir}/man3/explain_dup2.3
%{_mandir}/man3/explain_dup2_or_die.3
%{_mandir}/man3/explain_dup_or_die.3
%{_mandir}/man3/explain_endgrent.3
%{_mandir}/man3/explain_endgrent_or_die.3
%{_mandir}/man3/explain_eventfd.3
%{_mandir}/man3/explain_eventfd_or_die.3
%{_mandir}/man3/explain_execlp.3
%{_mandir}/man3/explain_execlp_or_die.3
%{_mandir}/man3/explain_execv.3
%{_mandir}/man3/explain_execv_or_die.3
%{_mandir}/man3/explain_execve.3
%{_mandir}/man3/explain_execve_or_die.3
%{_mandir}/man3/explain_execvp.3
%{_mandir}/man3/explain_execvp_or_die.3
%{_mandir}/man3/explain_exit.3
%{_mandir}/man3/explain_fchdir.3
%{_mandir}/man3/explain_fchdir_or_die.3
%{_mandir}/man3/explain_fchmod.3
%{_mandir}/man3/explain_fchmod_or_die.3
%{_mandir}/man3/explain_fchown.3
%{_mandir}/man3/explain_fchown_or_die.3
%{_mandir}/man3/explain_fchownat.3
%{_mandir}/man3/explain_fchownat_or_die.3
%{_mandir}/man3/explain_fclose.3
%{_mandir}/man3/explain_fclose_or_die.3
%{_mandir}/man3/explain_fcntl.3
%{_mandir}/man3/explain_fcntl_or_die.3
%{_mandir}/man3/explain_fdopen.3
%{_mandir}/man3/explain_fdopen_or_die.3
%{_mandir}/man3/explain_fdopendir.3
%{_mandir}/man3/explain_fdopendir_or_die.3
%{_mandir}/man3/explain_feof.3
%{_mandir}/man3/explain_feof_or_die.3
%{_mandir}/man3/explain_ferror.3
%{_mandir}/man3/explain_ferror_or_die.3
%{_mandir}/man3/explain_fflush.3
%{_mandir}/man3/explain_fflush_or_die.3
%{_mandir}/man3/explain_fgetc.3
%{_mandir}/man3/explain_fgetc_or_die.3
%{_mandir}/man3/explain_fgetpos.3
%{_mandir}/man3/explain_fgetpos_or_die.3
%{_mandir}/man3/explain_fgets.3
%{_mandir}/man3/explain_fgets_or_die.3
%{_mandir}/man3/explain_filename.3
%{_mandir}/man3/explain_fileno.3
%{_mandir}/man3/explain_fileno_or_die.3
%{_mandir}/man3/explain_flock.3
%{_mandir}/man3/explain_flock_or_die.3
%{_mandir}/man3/explain_fopen.3
%{_mandir}/man3/explain_fopen_or_die.3
%{_mandir}/man3/explain_fork.3
%{_mandir}/man3/explain_fork_or_die.3
%{_mandir}/man3/explain_fpathconf.3
%{_mandir}/man3/explain_fpathconf_or_die.3
%{_mandir}/man3/explain_fprintf.3
%{_mandir}/man3/explain_fprintf_or_die.3
%{_mandir}/man3/explain_fpurge.3
%{_mandir}/man3/explain_fpurge_or_die.3
%{_mandir}/man3/explain_fputc.3
%{_mandir}/man3/explain_fputc_or_die.3
%{_mandir}/man3/explain_fputs.3
%{_mandir}/man3/explain_fputs_or_die.3
%{_mandir}/man3/explain_fread.3
%{_mandir}/man3/explain_fread_or_die.3
%{_mandir}/man3/explain_freopen.3
%{_mandir}/man3/explain_freopen_or_die.3
%{_mandir}/man3/explain_fseek.3
%{_mandir}/man3/explain_fseek_or_die.3
%{_mandir}/man3/explain_fseeko.3
%{_mandir}/man3/explain_fseeko_or_die.3
%{_mandir}/man3/explain_fsetpos.3
%{_mandir}/man3/explain_fsetpos_or_die.3
%{_mandir}/man3/explain_fstat.3
%{_mandir}/man3/explain_fstat_or_die.3
%{_mandir}/man3/explain_fstatat.3
%{_mandir}/man3/explain_fstatat_or_die.3
%{_mandir}/man3/explain_fstatfs.3
%{_mandir}/man3/explain_fstatfs_or_die.3
%{_mandir}/man3/explain_fstatvfs.3
%{_mandir}/man3/explain_fstatvfs_or_die.3
%{_mandir}/man3/explain_fsync.3
%{_mandir}/man3/explain_fsync_or_die.3
%{_mandir}/man3/explain_ftell.3
%{_mandir}/man3/explain_ftell_or_die.3
%{_mandir}/man3/explain_ftello.3
%{_mandir}/man3/explain_ftello_or_die.3
%{_mandir}/man3/explain_ftime.3
%{_mandir}/man3/explain_ftime_or_die.3
%{_mandir}/man3/explain_ftruncate.3
%{_mandir}/man3/explain_ftruncate_or_die.3
%{_mandir}/man3/explain_futimens.3
%{_mandir}/man3/explain_futimens_or_die.3
%{_mandir}/man3/explain_futimes.3
%{_mandir}/man3/explain_futimes_or_die.3
%{_mandir}/man3/explain_futimesat.3
%{_mandir}/man3/explain_futimesat_or_die.3
%{_mandir}/man3/explain_fwrite.3
%{_mandir}/man3/explain_fwrite_or_die.3
%{_mandir}/man3/explain_getaddrinfo.3
%{_mandir}/man3/explain_getaddrinfo_or_die.3
%{_mandir}/man3/explain_getc.3
%{_mandir}/man3/explain_getc_or_die.3
%{_mandir}/man3/explain_getchar.3
%{_mandir}/man3/explain_getchar_or_die.3
%{_mandir}/man3/explain_getcwd.3
%{_mandir}/man3/explain_getcwd_or_die.3
%{_mandir}/man3/explain_getdomainname.3
%{_mandir}/man3/explain_getdomainname_or_die.3
%{_mandir}/man3/explain_getgrent.3
%{_mandir}/man3/explain_getgrent_or_die.3
%{_mandir}/man3/explain_getgrouplist.3
%{_mandir}/man3/explain_getgrouplist_or_die.3
%{_mandir}/man3/explain_getgroups.3
%{_mandir}/man3/explain_getgroups_or_die.3
%{_mandir}/man3/explain_gethostbyname.3
%{_mandir}/man3/explain_gethostbyname_or_die.3
%{_mandir}/man3/explain_gethostid.3
%{_mandir}/man3/explain_gethostid_or_die.3
%{_mandir}/man3/explain_gethostname.3
%{_mandir}/man3/explain_gethostname_or_die.3
%{_mandir}/man3/explain_getpeername.3
%{_mandir}/man3/explain_getpeername_or_die.3
%{_mandir}/man3/explain_getpgid.3
%{_mandir}/man3/explain_getpgid_or_die.3
%{_mandir}/man3/explain_getpgrp.3
%{_mandir}/man3/explain_getpgrp_or_die.3
%{_mandir}/man3/explain_getpriority.3
%{_mandir}/man3/explain_getpriority_or_die.3
%{_mandir}/man3/explain_getresgid.3
%{_mandir}/man3/explain_getresgid_or_die.3
%{_mandir}/man3/explain_getresuid.3
%{_mandir}/man3/explain_getresuid_or_die.3
%{_mandir}/man3/explain_getrlimit.3
%{_mandir}/man3/explain_getrlimit_or_die.3
%{_mandir}/man3/explain_getrusage.3
%{_mandir}/man3/explain_getrusage_or_die.3
%{_mandir}/man3/explain_getsockname.3
%{_mandir}/man3/explain_getsockname_or_die.3
%{_mandir}/man3/explain_getsockopt.3
%{_mandir}/man3/explain_getsockopt_or_die.3
%{_mandir}/man3/explain_gettimeofday.3
%{_mandir}/man3/explain_gettimeofday_or_die.3
%{_mandir}/man3/explain_getw.3
%{_mandir}/man3/explain_getw_or_die.3
%{_mandir}/man3/explain_iconv.3
%{_mandir}/man3/explain_iconv_close.3
%{_mandir}/man3/explain_iconv_close_or_die.3
%{_mandir}/man3/explain_iconv_open.3
%{_mandir}/man3/explain_iconv_open_or_die.3
%{_mandir}/man3/explain_iconv_or_die.3
%{_mandir}/man3/explain_ioctl.3
%{_mandir}/man3/explain_ioctl_or_die.3
%{_mandir}/man3/explain_kill.3
%{_mandir}/man3/explain_kill_or_die.3
%{_mandir}/man3/explain_lchmod.3
%{_mandir}/man3/explain_lchmod_or_die.3
%{_mandir}/man3/explain_lchown.3
%{_mandir}/man3/explain_lchown_or_die.3
%{_mandir}/man3/explain_lchownat.3
%{_mandir}/man3/explain_lchownat_or_die.3
%{_mandir}/man3/explain_license.3
%{_mandir}/man3/explain_link.3
%{_mandir}/man3/explain_link_or_die.3
%{_mandir}/man3/explain_linkat.3
%{_mandir}/man3/explain_linkat_or_die.3
%{_mandir}/man3/explain_listen.3
%{_mandir}/man3/explain_listen_or_die.3
%{_mandir}/man3/explain_lseek.3
%{_mandir}/man3/explain_lseek_or_die.3
%{_mandir}/man3/explain_lstat.3
%{_mandir}/man3/explain_lstat_or_die.3
%{_mandir}/man3/explain_lutimes.3
%{_mandir}/man3/explain_lutimes_or_die.3
%{_mandir}/man3/explain_malloc.3
%{_mandir}/man3/explain_malloc_or_die.3
%{_mandir}/man3/explain_mkdir.3
%{_mandir}/man3/explain_mkdir_or_die.3
%{_mandir}/man3/explain_mkdtemp.3
%{_mandir}/man3/explain_mkdtemp_or_die.3
%{_mandir}/man3/explain_mknod.3
%{_mandir}/man3/explain_mknod_or_die.3
%{_mandir}/man3/explain_mkostemp.3
%{_mandir}/man3/explain_mkostemp_or_die.3
%{_mandir}/man3/explain_mkstemp.3
%{_mandir}/man3/explain_mkstemp_or_die.3
%{_mandir}/man3/explain_mktemp.3
%{_mandir}/man3/explain_mktemp_or_die.3
%{_mandir}/man3/explain_mmap.3
%{_mandir}/man3/explain_mmap_or_die.3
%{_mandir}/man3/explain_mount.3
%{_mandir}/man3/explain_mount_or_die.3
%{_mandir}/man3/explain_munmap.3
%{_mandir}/man3/explain_munmap_or_die.3
%{_mandir}/man3/explain_nanosleep.3
%{_mandir}/man3/explain_nanosleep_or_die.3
%{_mandir}/man3/explain_nice.3
%{_mandir}/man3/explain_nice_or_die.3
%{_mandir}/man3/explain_open.3
%{_mandir}/man3/explain_open_or_die.3
%{_mandir}/man3/explain_openat.3
%{_mandir}/man3/explain_openat_or_die.3
%{_mandir}/man3/explain_opendir.3
%{_mandir}/man3/explain_opendir_or_die.3
%{_mandir}/man3/explain_output.3
%{_mandir}/man3/explain_pathconf.3
%{_mandir}/man3/explain_pathconf_or_die.3
%{_mandir}/man3/explain_pclose.3
%{_mandir}/man3/explain_pclose_or_die.3
%{_mandir}/man3/explain_pipe.3
%{_mandir}/man3/explain_pipe2.3
%{_mandir}/man3/explain_pipe2_or_die.3
%{_mandir}/man3/explain_pipe_or_die.3
%{_mandir}/man3/explain_poll.3
%{_mandir}/man3/explain_poll_or_die.3
%{_mandir}/man3/explain_popen.3
%{_mandir}/man3/explain_popen_or_die.3
%{_mandir}/man3/explain_pread.3
%{_mandir}/man3/explain_pread_or_die.3
%{_mandir}/man3/explain_printf.3
%{_mandir}/man3/explain_printf_or_die.3
%{_mandir}/man3/explain_program_name.3
%{_mandir}/man3/explain_ptrace.3
%{_mandir}/man3/explain_ptrace_or_die.3
%{_mandir}/man3/explain_putc.3
%{_mandir}/man3/explain_putc_or_die.3
%{_mandir}/man3/explain_putchar.3
%{_mandir}/man3/explain_putchar_or_die.3
%{_mandir}/man3/explain_putenv.3
%{_mandir}/man3/explain_putenv_or_die.3
%{_mandir}/man3/explain_puts.3
%{_mandir}/man3/explain_puts_or_die.3
%{_mandir}/man3/explain_putw.3
%{_mandir}/man3/explain_putw_or_die.3
%{_mandir}/man3/explain_pwrite.3
%{_mandir}/man3/explain_pwrite_or_die.3
%{_mandir}/man3/explain_raise.3
%{_mandir}/man3/explain_raise_or_die.3
%{_mandir}/man3/explain_read.3
%{_mandir}/man3/explain_read_or_die.3
%{_mandir}/man3/explain_readdir.3
%{_mandir}/man3/explain_readdir_or_die.3
%{_mandir}/man3/explain_readlink.3
%{_mandir}/man3/explain_readlink_or_die.3
%{_mandir}/man3/explain_readv.3
%{_mandir}/man3/explain_readv_or_die.3
%{_mandir}/man3/explain_realloc.3
%{_mandir}/man3/explain_realloc_or_die.3
%{_mandir}/man3/explain_realpath.3
%{_mandir}/man3/explain_realpath_or_die.3
%{_mandir}/man3/explain_remove.3
%{_mandir}/man3/explain_remove_or_die.3
%{_mandir}/man3/explain_rename.3
%{_mandir}/man3/explain_rename_or_die.3
%{_mandir}/man3/explain_rmdir.3
%{_mandir}/man3/explain_rmdir_or_die.3
%{_mandir}/man3/explain_select.3
%{_mandir}/man3/explain_select_or_die.3
%{_mandir}/man3/explain_setbuf.3
%{_mandir}/man3/explain_setbuf_or_die.3
%{_mandir}/man3/explain_setbuffer.3
%{_mandir}/man3/explain_setbuffer_or_die.3
%{_mandir}/man3/explain_setdomainname.3
%{_mandir}/man3/explain_setdomainname_or_die.3
%{_mandir}/man3/explain_setenv.3
%{_mandir}/man3/explain_setenv_or_die.3
%{_mandir}/man3/explain_setgid.3
%{_mandir}/man3/explain_setgid_or_die.3
%{_mandir}/man3/explain_setgrent.3
%{_mandir}/man3/explain_setgrent_or_die.3
%{_mandir}/man3/explain_setgroups.3
%{_mandir}/man3/explain_setgroups_or_die.3
%{_mandir}/man3/explain_sethostname.3
%{_mandir}/man3/explain_sethostname_or_die.3
%{_mandir}/man3/explain_setlinebuf.3
%{_mandir}/man3/explain_setlinebuf_or_die.3
%{_mandir}/man3/explain_setpgid.3
%{_mandir}/man3/explain_setpgid_or_die.3
%{_mandir}/man3/explain_setpgrp.3
%{_mandir}/man3/explain_setpgrp_or_die.3
%{_mandir}/man3/explain_setpriority.3
%{_mandir}/man3/explain_setpriority_or_die.3
%{_mandir}/man3/explain_setregid.3
%{_mandir}/man3/explain_setregid_or_die.3
%{_mandir}/man3/explain_setresgid.3
%{_mandir}/man3/explain_setresgid_or_die.3
%{_mandir}/man3/explain_setresuid.3
%{_mandir}/man3/explain_setresuid_or_die.3
%{_mandir}/man3/explain_setreuid.3
%{_mandir}/man3/explain_setreuid_or_die.3
%{_mandir}/man3/explain_setsid.3
%{_mandir}/man3/explain_setsid_or_die.3
%{_mandir}/man3/explain_setsockopt.3
%{_mandir}/man3/explain_setsockopt_or_die.3
%{_mandir}/man3/explain_settimeofday.3
%{_mandir}/man3/explain_settimeofday_or_die.3
%{_mandir}/man3/explain_setuid.3
%{_mandir}/man3/explain_setuid_or_die.3
%{_mandir}/man3/explain_setvbuf.3
%{_mandir}/man3/explain_setvbuf_or_die.3
%{_mandir}/man3/explain_shmat.3
%{_mandir}/man3/explain_shmat_or_die.3
%{_mandir}/man3/explain_shmctl.3
%{_mandir}/man3/explain_shmctl_or_die.3
%{_mandir}/man3/explain_signalfd.3
%{_mandir}/man3/explain_signalfd_or_die.3
%{_mandir}/man3/explain_sleep.3
%{_mandir}/man3/explain_sleep_or_die.3
%{_mandir}/man3/explain_snprintf.3
%{_mandir}/man3/explain_snprintf_or_die.3
%{_mandir}/man3/explain_socket.3
%{_mandir}/man3/explain_socket_or_die.3
%{_mandir}/man3/explain_socketpair.3
%{_mandir}/man3/explain_socketpair_or_die.3
%{_mandir}/man3/explain_sprintf.3
%{_mandir}/man3/explain_sprintf_or_die.3
%{_mandir}/man3/explain_stat.3
%{_mandir}/man3/explain_stat_or_die.3
%{_mandir}/man3/explain_statfs.3
%{_mandir}/man3/explain_statfs_or_die.3
%{_mandir}/man3/explain_statvfs.3
%{_mandir}/man3/explain_statvfs_or_die.3
%{_mandir}/man3/explain_stime.3
%{_mandir}/man3/explain_stime_or_die.3
%{_mandir}/man3/explain_strcoll.3
%{_mandir}/man3/explain_strcoll_or_die.3
%{_mandir}/man3/explain_strdup.3
%{_mandir}/man3/explain_strdup_or_die.3
%{_mandir}/man3/explain_strndup.3
%{_mandir}/man3/explain_strndup_or_die.3
%{_mandir}/man3/explain_strtod.3
%{_mandir}/man3/explain_strtod_or_die.3
%{_mandir}/man3/explain_strtof.3
%{_mandir}/man3/explain_strtof_or_die.3
%{_mandir}/man3/explain_strtol.3
%{_mandir}/man3/explain_strtol_or_die.3
%{_mandir}/man3/explain_strtold.3
%{_mandir}/man3/explain_strtold_or_die.3
%{_mandir}/man3/explain_strtoll.3
%{_mandir}/man3/explain_strtoll_or_die.3
%{_mandir}/man3/explain_strtoul.3
%{_mandir}/man3/explain_strtoul_or_die.3
%{_mandir}/man3/explain_strtoull.3
%{_mandir}/man3/explain_strtoull_or_die.3
%{_mandir}/man3/explain_symlink.3
%{_mandir}/man3/explain_symlink_or_die.3
%{_mandir}/man3/explain_system.3
%{_mandir}/man3/explain_system_or_die.3
%{_mandir}/man3/explain_tcdrain.3
%{_mandir}/man3/explain_tcdrain_or_die.3
%{_mandir}/man3/explain_tcflow.3
%{_mandir}/man3/explain_tcflow_or_die.3
%{_mandir}/man3/explain_tcflush.3
%{_mandir}/man3/explain_tcflush_or_die.3
%{_mandir}/man3/explain_tcgetattr.3
%{_mandir}/man3/explain_tcgetattr_or_die.3
%{_mandir}/man3/explain_tcsendbreak.3
%{_mandir}/man3/explain_tcsendbreak_or_die.3
%{_mandir}/man3/explain_tcsetattr.3
%{_mandir}/man3/explain_tcsetattr_or_die.3
%{_mandir}/man3/explain_telldir.3
%{_mandir}/man3/explain_telldir_or_die.3
%{_mandir}/man3/explain_tempnam.3
%{_mandir}/man3/explain_tempnam_or_die.3
%{_mandir}/man3/explain_time.3
%{_mandir}/man3/explain_time_or_die.3
%{_mandir}/man3/explain_timerfd_create.3
%{_mandir}/man3/explain_timerfd_create_or_die.3
%{_mandir}/man3/explain_tmpfile.3
%{_mandir}/man3/explain_tmpfile_or_die.3
%{_mandir}/man3/explain_tmpnam.3
%{_mandir}/man3/explain_tmpnam_or_die.3
%{_mandir}/man3/explain_truncate.3
%{_mandir}/man3/explain_truncate_or_die.3
%{_mandir}/man3/explain_uname.3
%{_mandir}/man3/explain_uname_or_die.3
%{_mandir}/man3/explain_ungetc.3
%{_mandir}/man3/explain_ungetc_or_die.3
%{_mandir}/man3/explain_unlink.3
%{_mandir}/man3/explain_unlink_or_die.3
%{_mandir}/man3/explain_unsetenv.3
%{_mandir}/man3/explain_unsetenv_or_die.3
%{_mandir}/man3/explain_usleep.3
%{_mandir}/man3/explain_usleep_or_die.3
%{_mandir}/man3/explain_ustat.3
%{_mandir}/man3/explain_ustat_or_die.3
%{_mandir}/man3/explain_utime.3
%{_mandir}/man3/explain_utime_or_die.3
%{_mandir}/man3/explain_utimens.3
%{_mandir}/man3/explain_utimens_or_die.3
%{_mandir}/man3/explain_utimensat.3
%{_mandir}/man3/explain_utimensat_or_die.3
%{_mandir}/man3/explain_utimes.3
%{_mandir}/man3/explain_utimes_or_die.3
%{_mandir}/man3/explain_vasprintf.3
%{_mandir}/man3/explain_vasprintf_or_die.3
%{_mandir}/man3/explain_vfork.3
%{_mandir}/man3/explain_vfork_or_die.3
%{_mandir}/man3/explain_vfprintf.3
%{_mandir}/man3/explain_vfprintf_or_die.3
%{_mandir}/man3/explain_vprintf.3
%{_mandir}/man3/explain_vprintf_or_die.3
%{_mandir}/man3/explain_vsnprintf.3
%{_mandir}/man3/explain_vsnprintf_or_die.3
%{_mandir}/man3/explain_vsprintf.3
%{_mandir}/man3/explain_vsprintf_or_die.3
%{_mandir}/man3/explain_wait.3
%{_mandir}/man3/explain_wait3.3
%{_mandir}/man3/explain_wait3_or_die.3
%{_mandir}/man3/explain_wait4.3
%{_mandir}/man3/explain_wait4_or_die.3
%{_mandir}/man3/explain_wait_or_die.3
%{_mandir}/man3/explain_waitpid.3
%{_mandir}/man3/explain_waitpid_or_die.3
%{_mandir}/man3/explain_write.3
%{_mandir}/man3/explain_write_or_die.3
%{_mandir}/man3/explain_writev.3
%{_mandir}/man3/explain_writev_or_die.3
%{_libdir}/libexplain.a
%{_libdir}/libexplain.so
