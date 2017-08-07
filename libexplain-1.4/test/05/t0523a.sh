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

TEST_SUBJECT="tmpfile EROFS"
. test_prelude

fmt > test.ok.1 << 'fubar'
tmpfile() failed, Read-only file system (EROFS) because write access
was requested and pathname refers to a file on a read-only file system
("/example", 42% full)
fubar
test $? -eq 0 || no_result

fmt > test.ok.2 << 'fubar'
tmpfile() failed, Read-only file system (EROFS) because write access
was requested and pathname refers to a file on a read-only file system
fubar
test $? -eq 0 || no_result

explain -eEROFS tmpfile > test.out.1 2>&1
test $? -eq 0 || fail

fmt -w 700 < test.out.1 > test.out.2
test $? -eq 0 || no_result

sed -e 's|("[^)]*full)|("/example", 42% full)|' \
    < test.out.2 > test.out.3
test $? -eq 0 || no_result

fmt < test.out.3 > test.out
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

# vim:ts=8:sw=4:et
