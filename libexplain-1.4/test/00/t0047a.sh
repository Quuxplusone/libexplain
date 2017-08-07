#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2008 Peter Miller
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

TEST_SUBJECT="read EINVAL"
. test_prelude

cat > test.ok << 'fubar'
read(fildes = 42, data = 0x00000100, data_size = 100) failed, Invalid
argument (EINVAL) because the file desriptor is attached to an object which
is unsuitable for reading; or, the file was opened with the O_DIRECT flag,
and either the address specified in data, the value specified in data_size,
or the current file offset is notsuitably aligned; or, the file descriptor
was created via a call to timerfd_create(2) and the wrong size buffer was
given
fubar
test $? -eq 0 || no_result

explain -e EINVAL -o test.out read 42 0x100 100
test $? -eq 0 || fail

diff test.ok test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim:ts=8:sw=4:et
