#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2009-2011 Peter Miller
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

TEST_SUBJECT="getpeername ENOBUFS"
. test_prelude

fmt > test.ok << 'fubar'
getpeername(fildes = 42, sock_addr = 0xNNNNNNNN, sock_addr_size = { XXX })
failed, No buffer space available (ENOBUFS) because insufficient
kernel resources are available in the system to complete the system call
fubar
test $? -eq 0 || no_result

explain -e ENOBUFS getpeername 42 > test.out.4
test $? -eq 0 || fail

fmt -w 700 test.out.4 > test.out.3
test $? -eq 0 || no_result

sed -e 's|0x[0-9a-fA-F][0-9a-fA-F]*|0xNNNNNNNN|g' \
    -e 's|size = { [0-9]* }|size = { XXX }|g' \
    test.out.3 > test.out.2
test $? -eq 0 || no_result

fmt test.out.2 > test.out
test $? -eq 0 || no_result

diff test.ok test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim:ts=8:sw=4:et
