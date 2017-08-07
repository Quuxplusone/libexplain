#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2008, 2011 Peter Miller
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

TEST_SUBJECT="ftruncate EFBIG"
. test_prelude

cat > test.ok << 'fubar'
ftruncate(fildes = 0, length = 74565) failed, File too large (EFBIG)
because length is larger than the maximum file size
fubar
test $? -eq 0 || no_result

explain -e EFBIG ftruncate 0 0x12345 > test.out4
test $? -eq 0 || fail

fmt -w 700 test.out4 > test.out3
test $? -eq 0 || no_result

sed -e 's|= 0 "[^"]*",|= 0,|' \
    -e 's| ([^)]*)$| (NNN)|' \
        test.out3 > test.out2
test $? -eq 0 || no_result

fmt test.out2 > test.out
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
