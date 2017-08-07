#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2008, 2010-2012 Peter Miller
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

TEST_SUBJECT="truncate EPERM"
. test_prelude

# this test doesn't work for root (or fakeroot)
test_config not-root || pass

fmt > test.ok.1 << 'fubar'
truncate(pathname = "foobar", length = 0) failed, Operation not permitted
(EPERM) because the underlying file system does not support extending a
file beyond its current size ("/example", 42% full)
fubar
test $? -eq 0 || no_result

fmt > test.ok.2 << 'fubar'
truncate(pathname = "foobar", length = 0) failed, Operation not permitted
(EPERM) because the underlying file system does not support extending a
file beyond its current size
fubar
test $? -eq 0 || no_result

touch foobar
test $? -eq 0 || no_result

explain -e EPERM truncate foobar 0 > test.out4
test $? -eq 0 || fail

fmt -w 700 test.out4 > test.out3
test $? -eq 0 || no_result

sed 's|([^)]* full)|("/example", 42% full)|' test.out3 > test.out2
test $? -eq 0 || no_result

fmt test.out2 > test.out
test $? -eq 0 || no_result

diff test.ok.2 test.out > /dev/null 2> /dev/null && pass

diff test.ok.1 test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
