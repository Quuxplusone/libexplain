#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2008-2011 Peter Miller
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

TEST_SUBJECT="explain_open vs ENOSPC"
. test_prelude

fmt > test.ok << 'fubar'
open(pathname = "fred", flags = O_WRONLY | O_CREAT | O_TRUNC, mode = S_IRUSR |
S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH) failed, No space left on
device (ENOSPC) because the file system containing pathname has no space
for a new directory entry
fubar
test $? -eq 0 || no_result

explain -e ENOSPC open fred O_WRONLY+O_CREAT+O_TRUNC > test.out4
test $? -eq 0 || fail

fmt -w 700 test.out4 > test.out3
test $? -eq 0 || no_result

sed 's| ("[^(),"]*", .*% full)||' test.out3 > test.out2
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
