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

TEST_SUBJECT="ungetc EBADF"
. test_prelude

cat > test.ok << 'fubar'
ungetc(c = '*', fp = 0x09876543) failed, Bad file descriptor (EBADF)
because the fp argument does not refer a valid file stream
fubar
test $? -eq 0 || no_result

explain -e EBADF ungetc 42 0x9876543 > test.out
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
