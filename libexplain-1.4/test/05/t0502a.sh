#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2009, 2010, 2014 Peter Miller
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

TEST_SUBJECT="tcsetattr EBADF"
. test_prelude

fmt > test.ok << 'fubar'
tcsetattr(fildes = 99, options = TCSANOW, data = { c_iflag = 0, c_oflag =
0, c_cflag = CS5, c_lflag = 0, c_line = N_TTY, c_cc = {} }) failed, Bad
file descriptor (EBADF) because the fildes argument does not refer to an
open file; this is more likely to be a software error (a bug) than it is to
be a user error
fubar
test $? -eq 0 || no_result

fmt > test.ok2 << 'fubar'
tcsetattr(fildes = 99, options = TCSANOW, data = { c_iflag = 0, c_oflag =
0, c_cflag = CS5, c_lflag = 0, c_cc = {} }) failed, Bad file descriptor
(EBADF) because the fildes argument does not refer to an open file; this
is more likely to be a software error (a bug) than it is to be a user error
fubar
test $? -eq 0 || no_result

explain -eEBADF tcsetattr 99 TCSANOW > test.out.2
test $? -eq 0 || fail

# so we can ignore linewrapping differences
fmt test.out.2 >  test.out
test $? -eq 0 || no_result

diff test.ok test.out > /dev/null 2> /dev/null || \
diff test.ok2 test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
