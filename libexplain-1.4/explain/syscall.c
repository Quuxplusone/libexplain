/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009-2013 Peter Miller
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

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>
#include <libexplain/ac/sys/acl.h>

#include <libexplain/sizeof.h>
#include <libexplain/fstrcmp.h>

#include <explain/syscall.h>
/* ----------  A  ------------------------------------------------------- */
#include <explain/syscall/accept.h>
#include <explain/syscall/accept4.h>
#include <explain/syscall/access.h>
#include <explain/syscall/acct.h>
#include <explain/syscall/acl_from_text.h>
#include <explain/syscall/acl_get_fd.h>
#include <explain/syscall/acl_get_file.h>
#include <explain/syscall/acl_set_fd.h>
#include <explain/syscall/acl_set_file.h>
#include <explain/syscall/acl_to_text.h>
#include <explain/syscall/adjtime.h>
#include <explain/syscall/adjtimex.h>
/* ----------  B  ------------------------------------------------------- */
#include <explain/syscall/bind.h>
/* ----------  C  ------------------------------------------------------- */
#include <explain/syscall/calloc.h>
#include <explain/syscall/chdir.h>
#include <explain/syscall/chmod.h>
#include <explain/syscall/chown.h>
#include <explain/syscall/chroot.h>
#include <explain/syscall/close.h>
#include <explain/syscall/closedir.h>
#include <explain/syscall/connect.h>
#include <explain/syscall/creat.h>
#include <explain/syscall/dirfd.h>
/* ----------  D  ------------------------------------------------------- */
#include <explain/syscall/dup.h>
#include <explain/syscall/dup2.h>
/* ----------  E  ------------------------------------------------------- */
#include <explain/syscall/endgrent.h>
#include <explain/syscall/eventfd.h>
#include <explain/syscall/execlp.h>
#include <explain/syscall/execv.h>
#include <explain/syscall/execve.h>
#include <explain/syscall/execvp.h>
/* ----------  F  ------------------------------------------------------- */
#include <explain/syscall/fchdir.h>
#include <explain/syscall/fchmod.h>
#include <explain/syscall/fchown.h>
#include <explain/syscall/fchownat.h>
#include <explain/syscall/fclose.h>
#include <explain/syscall/fcntl.h>
#include <explain/syscall/fdopen.h>
#include <explain/syscall/fdopendir.h>
#include <explain/syscall/feof.h>
#include <explain/syscall/ferror.h>
#include <explain/syscall/fflush.h>
#include <explain/syscall/fgetc.h>
#include <explain/syscall/fgetpos.h>
#include <explain/syscall/fgets.h>
#include <explain/syscall/fileno.h>
#include <explain/syscall/flock.h>
#include <explain/syscall/fopen.h>
#include <explain/syscall/fork.h>
#include <explain/syscall/fpathconf.h>
#include <explain/syscall/fpurge.h>
#include <explain/syscall/fputc.h>
#include <explain/syscall/fputs.h>
#include <explain/syscall/fread.h>
#include <explain/syscall/freopen.h>
#include <explain/syscall/fseek.h>
#include <explain/syscall/fseeko.h>
#include <explain/syscall/fsetpos.h>
#include <explain/syscall/fstat.h>
#include <explain/syscall/fstatat.h>
#include <explain/syscall/fstatfs.h>
#include <explain/syscall/fstatvfs.h>
#include <explain/syscall/fsync.h>
#include <explain/syscall/ftell.h>
#include <explain/syscall/ftello.h>
#include <explain/syscall/ftime.h>
#include <explain/syscall/ftruncate.h>
#include <explain/syscall/futimens.h>
#include <explain/syscall/futimes.h>
#include <explain/syscall/futimesat.h>
#include <explain/syscall/fwrite.h>
/* ----------  G  ------------------------------------------------------- */
#include <explain/syscall/getc.h>
#include <explain/syscall/getchar.h>
#include <explain/syscall/getcwd.h>
#include <explain/syscall/getdomainname.h>
#include <explain/syscall/getgrent.h>
#include <explain/syscall/getgroups.h>
#include <explain/syscall/getgrouplist.h>
#include <explain/syscall/gethostbyname.h>
#include <explain/syscall/gethostid.h>
#include <explain/syscall/gethostname.h>
#include <explain/syscall/getpeername.h>
#include <explain/syscall/getpgid.h>
#include <explain/syscall/getpgrp.h>
#include <explain/syscall/getpriority.h>
#include <explain/syscall/getresgid.h>
#include <explain/syscall/getresuid.h>
#include <explain/syscall/getrlimit.h>
#include <explain/syscall/getrusage.h>
#include <explain/syscall/getsockname.h>
#include <explain/syscall/getsockopt.h>
#include <explain/syscall/gettimeofday.h>
#include <explain/syscall/getw.h>
/* ----------  H  ------------------------------------------------------- */
/* ----------  I  ------------------------------------------------------- */
#include <explain/syscall/ioctl.h>
#include <explain/syscall/iconv.h>
#include <explain/syscall/iconv_close.h>
#include <explain/syscall/iconv_open.h>
/* ----------  J  ------------------------------------------------------- */
/* ----------  K  ------------------------------------------------------- */
#include <explain/syscall/kill.h>
/* ----------  L  ------------------------------------------------------- */
#include <explain/syscall/lchmod.h>
#include <explain/syscall/lchown.h>
#include <explain/syscall/lchownat.h>
#include <explain/syscall/link.h>
#include <explain/syscall/linkat.h>
#include <explain/syscall/listen.h>
#include <explain/syscall/lseek.h>
#include <explain/syscall/lstat.h>
#include <explain/syscall/lutimes.h>
/* ----------  M  ------------------------------------------------------- */
#include <explain/syscall/malloc.h>
#include <explain/syscall/mkdir.h>
#include <explain/syscall/mkdtemp.h>
#include <explain/syscall/mknod.h>
#include <explain/syscall/mkostemp.h>
#include <explain/syscall/mkstemp.h>
#include <explain/syscall/mktemp.h>
#include <explain/syscall/mmap.h>
#include <explain/syscall/mount.h>
#include <explain/syscall/munmap.h>
/* ----------  N  ------------------------------------------------------- */
#include <explain/syscall/nanosleep.h>
#include <explain/syscall/nice.h>
/* ----------  O  ------------------------------------------------------- */
#include <explain/syscall/openat.h>
#include <explain/syscall/opendir.h>
#include <explain/syscall/open.h>
#include <explain/syscall/pathconf.h>
/* ----------  P  ------------------------------------------------------- */
#include <explain/syscall/pclose.h>
#include <explain/syscall/pipe.h>
#include <explain/syscall/pipe2.h>
#include <explain/syscall/poll.h>
#include <explain/syscall/popen.h>
#include <explain/syscall/pread.h>
#include <explain/syscall/ptrace.h>
#include <explain/syscall/putc.h>
#include <explain/syscall/putchar.h>
#include <explain/syscall/putenv.h>
#include <explain/syscall/puts.h>
#include <explain/syscall/putw.h>
#include <explain/syscall/pwrite.h>
/* ----------  Q  ------------------------------------------------------- */
/* ----------  R  ------------------------------------------------------- */
#include <explain/syscall/raise.h>
#include <explain/syscall/read.h>
#include <explain/syscall/readdir.h>
#include <explain/syscall/readlink.h>
#include <explain/syscall/readv.h>
#include <explain/syscall/realloc.h>
#include <explain/syscall/realpath.h>
#include <explain/syscall/remove.h>
#include <explain/syscall/rename.h>
#include <explain/syscall/rmdir.h>
/* ----------  S  ------------------------------------------------------- */
#include <explain/syscall/select.h>
#include <explain/syscall/setbuf.h>
#include <explain/syscall/setbuffer.h>
#include <explain/syscall/setdomainname.h>
#include <explain/syscall/setenv.h>
#include <explain/syscall/setgid.h>
#include <explain/syscall/setgrent.h>
#include <explain/syscall/setgroups.h>
#include <explain/syscall/sethostname.h>
#include <explain/syscall/setlinebuf.h>
#include <explain/syscall/setpgid.h>
#include <explain/syscall/setpgrp.h>
#include <explain/syscall/setpriority.h>
#include <explain/syscall/setregid.h>
#include <explain/syscall/setresgid.h>
#include <explain/syscall/setresuid.h>
#include <explain/syscall/setreuid.h>
#include <explain/syscall/setreuid.h>
#include <explain/syscall/setsid.h>
#include <explain/syscall/setsockopt.h>
#include <explain/syscall/settimeofday.h>
#include <explain/syscall/setuid.h>
#include <explain/syscall/setvbuf.h>
#include <explain/syscall/shmat.h>
#include <explain/syscall/shmctl.h>
#include <explain/syscall/signalfd.h>
#include <explain/syscall/sleep.h>
#include <explain/syscall/sleep.h>
#include <explain/syscall/socket.h>
#include <explain/syscall/socketpair.h>
#include <explain/syscall/stat.h>
#include <explain/syscall/statfs.h>
#include <explain/syscall/statvfs.h>
#include <explain/syscall/stime.h>
#include <explain/syscall/strcoll.h>
#include <explain/syscall/strdup.h>
#include <explain/syscall/strerror.h>
#include <explain/syscall/strndup.h>
#include <explain/syscall/strtod.h>
#include <explain/syscall/strtof.h>
#include <explain/syscall/strtol.h>
#include <explain/syscall/strtold.h>
#include <explain/syscall/strtoll.h>
#include <explain/syscall/strtoul.h>
#include <explain/syscall/strtoull.h>
#include <explain/syscall/symlink.h>
#include <explain/syscall/system.h>
/* ----------  T  ------------------------------------------------------- */
#include <explain/syscall/tcdrain.h>
#include <explain/syscall/tcflow.h>
#include <explain/syscall/tcflush.h>
#include <explain/syscall/tcgetattr.h>
#include <explain/syscall/tcsendbreak.h>
#include <explain/syscall/tcsetattr.h>
#include <explain/syscall/telldir.h>
#include <explain/syscall/tempnam.h>
#include <explain/syscall/time.h>
#include <explain/syscall/timerfd_create.h>
#include <explain/syscall/tmpfile.h>
#include <explain/syscall/tmpnam.h>
#include <explain/syscall/truncate.h>
/* ----------  U  ------------------------------------------------------- */
#include <explain/syscall/uname.h>
#include <explain/syscall/ungetc.h>
#include <explain/syscall/unlink.h>
#include <explain/syscall/usleep.h>
#include <explain/syscall/unsetenv.h>
#include <explain/syscall/ustat.h>
#include <explain/syscall/utime.h>
#include <explain/syscall/utimens.h>
#include <explain/syscall/utimensat.h>
#include <explain/syscall/utimes.h>
/* ----------  V  ------------------------------------------------------- */
#include <explain/syscall/vfork.h>
/* ----------  W  ------------------------------------------------------- */
#include <explain/syscall/wait.h>
#include <explain/syscall/wait3.h>
#include <explain/syscall/wait4.h>
#include <explain/syscall/waitpid.h>
#include <explain/syscall/write.h>
#include <explain/syscall/writev.h>
/* ----------  X  ------------------------------------------------------- */
/* ----------  Y  ------------------------------------------------------- */
/* ----------  Z  ------------------------------------------------------- */


typedef struct table_t table_t;
struct table_t
{
    const char      *name;
    func_t          func;
};


static const table_t table[] =
{
    /* ----------  A  ------------------------------------------------------- */
    { "accept", explain_syscall_accept },
    { "accept4", explain_syscall_accept4 },
    { "access", explain_syscall_access },
    { "acct", explain_syscall_acct },
    { "adjtime", explain_syscall_adjtime },
    { "adjtimex", explain_syscall_adjtimex },
    { "acl_from_text", explain_syscall_acl_from_text },
    { "acl_get_fd", explain_syscall_acl_get_fd },
    { "acl_get_file", explain_syscall_acl_get_file },
    { "acl_set_fd", explain_syscall_acl_set_fd },
    { "acl_set_file", explain_syscall_acl_set_file },
    { "acl_to_text", explain_syscall_acl_to_text },
    /* FIXME: add support for afs_syscall */
    /* FIXME: add support for alloc_hugepages */
    /* ----------  B  ------------------------------------------------------- */
    /* FIXME: add support for bdflush */
    { "bind", explain_syscall_bind },
    /* FIXME: add support for break */
    /* FIXME: add support for brk */
    /* ----------  C  ------------------------------------------------------- */
    /* FIXME: add support for cacheflush */
    { "calloc", explain_syscall_calloc },
    /* FIXME: add support for capget */
    /* FIXME: add support for capset */
    { "chdir", explain_syscall_chdir },
    { "chmod", explain_syscall_chmod },
    { "chown", explain_syscall_chown },
    { "chroot", explain_syscall_chroot },
    /* FIXME: add support for clock_getres */
    /* FIXME: add support for clock_gettime */
    /* FIXME: add support for clock_nanosleep */
    /* FIXME: add support for clock_settime */
    /* FIXME: add support for clone */
    { "close", explain_syscall_close },
    { "closedir", explain_syscall_closedir },
    { "connect", explain_syscall_connect },
    { "creat", explain_syscall_creat },
    /* FIXME: add support for create_module */
    /* ----------  D  ------------------------------------------------------- */
    /* FIXME: add support for delete_module */
    { "dirfd", explain_syscall_dirfd },
    { "dup", explain_syscall_dup },
    { "dup2", explain_syscall_dup2 },
    /* ----------  E  ------------------------------------------------------- */
    /* FIXME: add support for epoll_create */
    /* FIXME: add support for epoll_ctl */
    /* FIXME: add support for epoll_pwait */
    /* FIXME: add support for epoll_wait */
    { "endgrent", explain_syscall_endgrent },
    { "eventfd", explain_syscall_eventfd },
    /* FIXME: add support for execl */
    /* FIXME: add support for execle */
    { "execlp", explain_syscall_execlp },
    { "execv", explain_syscall_execv },
    { "execve", explain_syscall_execve },
    { "execvp", explain_syscall_execvp },
    /* ----------  F  ------------------------------------------------------- */
    /* FIXME: add support for faccess */
    /* FIXME: add support for fadvise64 */
    /* FIXME: add support for fallocate */
    { "fchdir", explain_syscall_fchdir },
    { "fchmod", explain_syscall_fchmod },
    { "fchown", explain_syscall_fchown },
    { "fchownat", explain_syscall_fchownat },
    { "fclose", explain_syscall_fclose },
    { "fcntl", explain_syscall_fcntl },
    /* FIXME: add support for fdatasync */
    { "fdopen", explain_syscall_fdopen },
    { "openat", explain_syscall_openat },
    { "fdopendir", explain_syscall_fdopendir },
    { "feof", explain_syscall_feof },
    { "ferror", explain_syscall_ferror },
    { "fflush", explain_syscall_fflush },
    { "fgetc", explain_syscall_fgetc },
    { "fgetpos", explain_syscall_fgetpos },
    { "fgets", explain_syscall_fgets },
    /* FIXME: add support for fgetxattr */
    { "fileno", explain_syscall_fileno },
    /* FIXME: add support for flistxattr */
    { "flock", explain_syscall_flock },
    { "fopen", explain_syscall_fopen },
    { "fork", explain_syscall_fork },
    { "fpathconf", explain_syscall_fpathconf },
    { "fpurge", explain_syscall_fpurge },
    { "fputc", explain_syscall_fputc },
    { "fputs", explain_syscall_fputs },
    { "fread", explain_syscall_fread },
    /* FIXME: add support for free_hugepages */
    { "freopen", explain_syscall_freopen },
    /* FIXME: add support for fremovexattr */
    /* FIXME: add support for fscanf */
    { "fseek", explain_syscall_fseek },
    { "fseeko", explain_syscall_fseeko },
    { "fsetpos", explain_syscall_fsetpos },
    /* FIXME: add support for fsetxattr */
    { "fstat", explain_syscall_fstat },
    { "fstatat", explain_syscall_fstatat },
    { "fstatfs", explain_syscall_fstatfs },
    { "fstatvfs", explain_syscall_fstatvfs },
    { "fsync", explain_syscall_fsync },
    { "ftell", explain_syscall_ftell },
    { "ftello", explain_syscall_ftello },
    { "ftime", explain_syscall_ftime },
    { "ftruncate", explain_syscall_ftruncate },
    /* FIXME: add support for futex */
    { "futimens", explain_syscall_futimens },
    { "futimes", explain_syscall_futimes },
    { "futimesat", explain_syscall_futimesat },
    { "fwrite", explain_syscall_fwrite },
    /* ----------  G  ------------------------------------------------------- */
    { "getc", explain_syscall_getc },
    { "getchar", explain_syscall_getchar },
    /* FIXME: add support for getcpu */
    { "getcwd", explain_syscall_getcwd },
    /* FIXME: add support for getdents */
    { "getdomainname", explain_syscall_getdomainname },
    { "getgrent", explain_syscall_getgrent },
    { "getgrouplist", explain_syscall_getgrouplist },
    { "getgroups", explain_syscall_getgroups },
    { "gethostbyname", explain_syscall_gethostbyname },
    { "gethostid", explain_syscall_gethostid },
    { "gethostname", explain_syscall_gethostname },
    /* FIXME: add support for getitimer */
    /* FIXME: add support for get_kernel_syms */
    /* FIXME: add support for get_mempolicy */
    /* FIXME: add support for getpagesize */
    { "getpeername", explain_syscall_getpeername },
    { "getpgid", explain_syscall_getpgid },
    { "getpgrp", explain_syscall_getpgrp },
    /* FIXME: add support for getpmsg */
    { "getpriority", explain_syscall_getpriority },
    { "getresgid", explain_syscall_getresgid },
    { "getresuid", explain_syscall_getresuid },
    { "getrlimit", explain_syscall_getrlimit },
    /* FIXME: add support for get_robust_list */
    { "getrusage", explain_syscall_getrusage },
    /* FIXME: add support for gets */
    /* FIXME: add support for getsid */
    { "getsockname", explain_syscall_getsockname },
    { "getsockopt", explain_syscall_getsockopt },
    /* FIXME: add support for get_thread_area */
    /* FIXME: add support for gettid */
    { "gettimeofday", explain_syscall_gettimeofday },
    { "getw", explain_syscall_getw },
    /* FIXME: add support for getxattr */
    /* FIXME: add support for gtty */
    /* ----------  H  ------------------------------------------------------- */
    /* ----------  I  ------------------------------------------------------- */
    { "iconv", explain_syscall_iconv },
    { "iconv_close", explain_syscall_iconv_close },
    { "iconv_open", explain_syscall_iconv_open },
    /* FIXME: add support for idle */
    /* FIXME: add support for init_module */
    /* FIXME: add support for inotify_add_watch */
    /* FIXME: add support for inotify_init */
    /* FIXME: add support for inotify_rm_watch */
    /* FIXME: add support for io_cancel */
    { "ioctl", explain_syscall_ioctl },
    /* FIXME: add support for io_destroy */
    /* FIXME: add support for io_getevents */
    /* FIXME: add support for ioperm */
    /* FIXME: add support for iopl */
    /* FIXME: add support for ioprio_get */
    /* FIXME: add support for ioprio_set */
    /* FIXME: add support for io_setup */
    /* FIXME: add support for io_submit */
    /* FIXME: add support for ipc */
    /* ----------  J  ------------------------------------------------------- */
    /* ----------  K  ------------------------------------------------------- */
    /* FIXME: add support for kexec_load */
    /* FIXME: add support for keyctl */
    { "kill", explain_syscall_kill },
    /* ----------  L  ------------------------------------------------------- */
    { "lchmod", explain_syscall_lchmod },
    { "lchown", explain_syscall_lchown },
    { "lchownat", explain_syscall_lchownat },
    /* FIXME: add support for lgetxattr */
    { "link", explain_syscall_link },
    { "linkat", explain_syscall_linkat },
    { "listen", explain_syscall_listen },
    /* FIXME: add support for listxattr */
    /* FIXME: add support for llistxattr */
    /* FIXME: add support for lock */
    /* FIXME: add support for lookup_dcookie */
    /* FIXME: add support for lremovexattr */
    { "lseek", explain_syscall_lseek },
    /* FIXME: add support for lsetxattr */
    { "lstat", explain_syscall_lstat },
    { "lutimes", explain_syscall_lutimes },
    /* ----------  M  ------------------------------------------------------- */
    /* FIXME: add support for madvise */
    /* FIXME: add support for madvise1 */
    { "malloc", explain_syscall_malloc },
    /* FIXME: add support for mbind */
    /* FIXME: add support for migrate_pages */
    /* FIXME: add support for mincore */
    { "mkdir", explain_syscall_mkdir },
    { "mkdtemp", explain_syscall_mkdtemp },
    { "mknod", explain_syscall_mknod },
    { "mkostemp", explain_syscall_mkostemp },
    { "mkstemp", explain_syscall_mkstemp },
    { "mktemp", explain_syscall_mktemp },
    /* FIXME: add support for mlock */
    /* FIXME: add support for mlockall */
    { "mmap", explain_syscall_mmap },
    /* FIXME: add support for mmap2 */
    /* FIXME: add support for modify_ldt */
    { "mount", explain_syscall_mount },
    /* FIXME: add support for move_pages */
    /* FIXME: add support for mprotect */
    /* FIXME: add support for mpx */
    /* FIXME: add support for mq_getsetattr */
    /* FIXME: add support for mq_notify */
    /* FIXME: add support for mq_open */
    /* FIXME: add support for mq_timedreceive */
    /* FIXME: add support for mq_timedsend */
    /* FIXME: add support for mq_unlink */
    /* FIXME: add support for mremap */
    /* FIXME: add support for msgctl */
    /* FIXME: add support for msgget */
    /* FIXME: add support for msgrcv */
    /* FIXME: add support for msgsnd */
    /* FIXME: add support for msync */
    /* FIXME: add support for munlock */
    /* FIXME: add support for munlockall */
    { "munmap", explain_syscall_munmap },
    /* ----------  N  ------------------------------------------------------- */
    { "nanosleep", explain_syscall_nanosleep },
    /* FIXME: add support for nfsservctl */
    { "nice", explain_syscall_nice },
    /* ----------  O  ------------------------------------------------------- */
    { "open", explain_syscall_open },
    { "opendir", explain_syscall_opendir },
    /* ----------  P  ------------------------------------------------------- */
    { "pathconf", explain_syscall_pathconf },
    /* FIXME: add support for pause */
    /* FIXME: add support for pciconfig_iobase */
    /* FIXME: add support for pciconfig_read */
    /* FIXME: add support for pciconfig_write */
    { "pclose", explain_syscall_pclose },
    /* FIXME: add support for perror */
    /* FIXME: add support for personality */
    /* FIXME: add support for phys */
    { "pipe", explain_syscall_pipe },
    { "pipe2", explain_syscall_pipe2 },
    /* FIXME: add support for pivot_root */
    { "poll", explain_syscall_poll },
    { "popen", explain_syscall_popen },
    /* FIXME: add support for ppoll */
    /* FIXME: add support for prctl */
    { "pread", explain_syscall_pread },
    /* FIXME: add support for prof */
    /* FIXME: add support for profil */
    /* FIXME: add support for pselect6 */
    { "ptrace", explain_syscall_ptrace },
    { "putc", explain_syscall_putc },
    { "putchar", explain_syscall_putchar },
    { "putenv", explain_syscall_putenv },
    /* FIXME: add support for putpmsg */
    { "puts", explain_syscall_puts },
    { "putw", explain_syscall_putw },
    { "pwrite", explain_syscall_pwrite },
    /* ----------  Q  ------------------------------------------------------- */
    /* FIXME: add support for query_module */
    /* FIXME: add support for quotactl */
    /* ----------  R  ------------------------------------------------------- */
    { "raise", explain_syscall_raise },
    { "read", explain_syscall_read },
    /* FIXME: add support for readahead */
    { "readdir", explain_syscall_readdir },
    { "readlink", explain_syscall_readlink },
    { "readv", explain_syscall_readv },
    { "realloc", explain_syscall_realloc },
    { "realpath", explain_syscall_realpath },
    /* FIXME: add support for reboot */
    /* FIXME: add support for recv */
    /* FIXME: add support for recvfrom */
    /* FIXME: add support for recvmsg */
    /* FIXME: add support for remap_file_pages */
    { "remove", explain_syscall_remove },
    /* FIXME: add support for removexattr */
    { "rename", explain_syscall_rename },
    /* FIXME: add support for request_key */
    /* FIXME: add support for restart_syscall */
    /* FIXME: add support for rewind */
    { "rmdir", explain_syscall_rmdir },
    /* FIXME: add support for rt_sigaction */
    /* FIXME: add support for rt_sigpending */
    /* FIXME: add support for rt_sigprocmask */
    /* FIXME: add support for rt_sigqueueinfo */
    /* FIXME: add support for rt_sigreturn */
    /* FIXME: add support for rt_sigsuspend */
    /* FIXME: add support for rt_sigtimedwait */
    /* ----------  S  ------------------------------------------------------- */
    /* FIXME: add support for scandir */
    /* FIXME: add support for scanf */
    /* FIXME: add support for sched_getaffinity */
    /* FIXME: add support for sched_getparam */
    /* FIXME: add support for sched_get_priority_max */
    /* FIXME: add support for sched_get_priority_min */
    /* FIXME: add support for sched_getscheduler */
    /* FIXME: add support for sched_rr_get_interval */
    /* FIXME: add support for sched_setaffinity */
    /* FIXME: add support for sched_setparam */
    /* FIXME: add support for sched_setscheduler */
    /* FIXME: add support for sched_yield */
    /* FIXME: add support for security */
    { "select", explain_syscall_select },
    /* FIXME: add support for semctl */
    /* FIXME: add support for semget */
    /* FIXME: add support for semop */
    /* FIXME: add support for semtimedop */
    /* FIXME: add support for send */
    /* FIXME: add support for sendfile */
    /* FIXME: add support for sendmsg */
    /* FIXME: add support for sendto */
    { "setbuf", explain_syscall_setbuf },
    { "setbuffer", explain_syscall_setbuffer },
    { "setdomainname", explain_syscall_setdomainname },
    { "setenv", explain_syscall_setenv },
    /* FIXME: add support for setfsgid */
    /* FIXME: add support for setfsuid */
    { "setgid", explain_syscall_setgid },
    { "setgrent", explain_syscall_setgrent },
    { "setgroups", explain_syscall_setgroups },
    { "sethostname", explain_syscall_sethostname },
    /* FIXME: add support for setitimer */
    { "setlinebuf", explain_syscall_setlinebuf },
    /* FIXME: add support for set_mempolicy */
    { "setpgid", explain_syscall_setpgid },
    { "setpgrp", explain_syscall_setpgrp },
    { "setpriority", explain_syscall_setpriority },
    { "setregid", explain_syscall_setregid },
    { "setresgid", explain_syscall_setresgid },
    { "setresuid", explain_syscall_setresuid },
    { "setreuid", explain_syscall_setreuid },
    /* FIXME: add support for setrlimit */
    /* FIXME: add support for set_robust_list */
    { "setsid", explain_syscall_setsid },
    { "setsockopt", explain_syscall_setsockopt },
    /* FIXME: add support for set_thread_area */
    /* FIXME: add support for set_tid_address */
    { "settimeofday", explain_syscall_settimeofday },
    { "setuid", explain_syscall_setuid },
    /* FIXME: add support for setup */
    { "setvbuf", explain_syscall_setvbuf },
    /* FIXME: add support for setxattr */
    /* FIXME: add support for set_zone_reclaim */
    /* FIXME: add support for sgetmask */
    { "shmat", explain_syscall_shmat },
    { "shmctl", explain_syscall_shmctl },
    /* FIXME: add support for shmdt */
    /* FIXME: add support for shmget */
    /* FIXME: add support for shutdown */
    /* FIXME: add support for sigaction */
    /* FIXME: add support for sigaltstack */
    /* FIXME: add support for signal */
    { "signalfd", explain_syscall_signalfd },
    /* FIXME: add support for sigpending */
    /* FIXME: add support for sigprocmask */
    /* FIXME: add support for sigreturn */
    /* FIXME: add support for sigsuspend */
    { "sleep", explain_syscall_sleep },
    { "socket", explain_syscall_socket },
    /* FIXME: add support for socketcall */
    { "socketpair", explain_syscall_socketpair },
    /* FIXME: add support for splice */
    /* FIXME: add support for sprintf */
    /* FIXME: add support for spu_create */
    /* FIXME: add support for spu_run */
    /* FIXME: add support for sscanf */
    /* FIXME: add support for ssetmask */
    { "stat", explain_syscall_stat },
    { "statfs", explain_syscall_statfs },
    { "statvfs", explain_syscall_statvfs },
    { "stime", explain_syscall_stime },
    { "strcoll", explain_syscall_strcoll },
    { "strdup", explain_syscall_strdup },
    { "strerror", explain_syscall_strerror },
    { "strndup", explain_syscall_strndup },
    { "strtod", explain_syscall_strtod },
    { "strtof", explain_syscall_strtof },
    { "strtol", explain_syscall_strtol },
    { "strtold", explain_syscall_strtold },
    { "strtoll", explain_syscall_strtoll },
    { "strtoul", explain_syscall_strtoul },
    { "strtoull", explain_syscall_strtoull },
    /* FIXME: add support for stty */
    /* FIXME: add support for swapoff */
    /* FIXME: add support for swapon */
    { "symlink", explain_syscall_symlink },
    /* FIXME: add support for sync */
    /* FIXME: add support for sync_file_range */
    /* FIXME: add support for sysfs */
    /* FIXME: add support for sysinfo */
    /* FIXME: add support for syslog */
    { "system", explain_syscall_system },
    /* ----------  T  ------------------------------------------------------- */
    { "tcdrain", explain_syscall_tcdrain },
    { "tcflow", explain_syscall_tcflow },
    { "tcflush", explain_syscall_tcflush },
    { "tcgetattr", explain_syscall_tcgetattr },
    { "tcsendbreak", explain_syscall_tcsendbreak },
    { "tcsetattr", explain_syscall_tcsetattr },
    /* FIXME: add support for tee */
    { "telldir", explain_syscall_telldir },
    { "tempnam", explain_syscall_tempnam },
    /* FIXME: add support for tgkill */
    { "time", explain_syscall_time },
    /* FIXME: add support for timer_create */
    /* FIXME: add support for timer_delete */
    { "timerfd_create", explain_syscall_timerfd_create },
    /* FIXME: add support for timerfd_gettime */
    /* FIXME: add support for timerfd_settime */
    /* FIXME: add support for timer_getoverrun */
    /* FIXME: add support for timer_gettime */
    /* FIXME: add support for timer_settime */
    /* FIXME: add support for times */
    /* FIXME: add support for tkill */
    { "tmpfile", explain_syscall_tmpfile },
    { "tmpnam", explain_syscall_tmpnam },
    { "truncate", explain_syscall_truncate },
    /* FIXME: add support for tuxcall */
    /* ----------  U  ------------------------------------------------------- */
    /* FIXME: add support for ugetrlimit */
    /* FIXME: add support for ulimit */
    /* FIXME: add support for umount */
    { "uname", explain_syscall_uname },
    { "usleep", explain_syscall_usleep },
    { "ungetc", explain_syscall_ungetc },
    { "unlink", explain_syscall_unlink },
    { "unsetenv", explain_syscall_unsetenv },
    /* FIXME: add support for unshare */
    /* FIXME: add support for uselib */
    { "ustat", explain_syscall_ustat },
    { "utime", explain_syscall_utime },
    { "utimens", explain_syscall_utimens },
    { "utimensat", explain_syscall_utimensat },
    { "utimes", explain_syscall_utimes },
    /* ----------  V  ------------------------------------------------------- */
    { "vfork", explain_syscall_vfork },
    /* FIXME: add support for vfscanf */
    /* FIXME: add support for vhangup */
    /* FIXME: add support for vmsplice */
    /* FIXME: add support for vscanf */
    /* FIXME: add support for vserver */
    /* FIXME: add support for vsscanf */
    /* ----------  W  ------------------------------------------------------- */
    { "wait", explain_syscall_wait },
    { "wait3", explain_syscall_wait3 },
    { "wait4", explain_syscall_wait4 },
    /* FIXME: add support for waitid */
    { "waitpid", explain_syscall_waitpid },
    { "write", explain_syscall_write },
    { "writev", explain_syscall_writev },
    /* ----------  X  ------------------------------------------------------- */
    /* ----------  Y  ------------------------------------------------------- */
    /* ----------  Z  ------------------------------------------------------- */
};


func_t
find_function(const char *name)
{
    const table_t   *tp;
    const table_t   *best_tp;
    double          best_weight;

    for (tp = table; tp < ENDOF(table); ++tp)
    {
        if (0 == strcasecmp(name, tp->name))
            return tp->func;
    }

    best_tp = 0;
    best_weight = 0.6;
    for (tp = table; tp < ENDOF(table); ++tp)
    {
        double          weight;

        weight = explain_fstrcmp(name, tp->name);
        if (best_weight < weight)
        {
            best_tp = tp;
            best_weight = weight;
        }
    }
    if (best_tp)
    {
        fprintf
        (
            stderr,
            "function \"%s\" unknown, did you mean the \"%s\" function?\n",
            name,
            best_tp->name
        );
        exit(EXIT_FAILURE);
        return best_tp->func;
    }
    else
        fprintf(stderr, "function \"%s\" unknown\n", name);
    exit(EXIT_FAILURE);
    return 0;
}


void
syscall_statistics(int *total)
{
    *total = SIZEOF(table);
}


/* vim: set ts=8 sw=4 et : */
