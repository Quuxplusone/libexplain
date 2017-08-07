#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2010 Peter Miller
# Written by Peter Miller <pmiller@opensource.org.au>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or (at
# your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program. If not, see <http://www.gnu.org/licenses/>.
#

case $# in
2)
    ;;
*)
    echo "Usage: <syscall.table.c> <ioctrol.table.c>" 1>&2
    exit 1
    ;;
esac

syscalls=$1
num_syscalls=`awk '/^    { ".*explain_syscall_/{++n}END{print n}' $syscalls`
echo "Coverage includes ${num_syscalls} system calls"

ioctls=$2
num_ioctls=`awk '/^    &explain_iocontrol_.*,$/{++n}END{print n}' $ioctls`
echo "and ${num_ioctls} ioctl requests."

exit 0
