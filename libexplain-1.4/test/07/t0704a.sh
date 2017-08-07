#!/bin/sh
#
# libexplain - a library of system-call-specific strerror replacements
# Copyright (C) 2013 Peter Miller
# Written by Peter Miller <pmiller@opensource.org.au>
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 3 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along with
# this program. If not, see <http://www.gnu.org/licenses/>.
#

TEST_SUBJECT="fchownat ENOTDIR"
. test_prelude

mkdir -p a/b/c
test $? -eq 0 || no_result

fmt > test.ok << 'fubar'
fchownat(fildes = 3 "a", pathname = "rat/fink", uid = -1, gid = -1,
flags = 0) failed, Not a directory (ENOTDIR) because there is no "rat"
directory in the pathname "a" directory
fubar
test $? -eq 0 || no_result

explain -eENOTDIR fchownat a rat/fink  > test.out.4
test $? -eq 0 || fail

fmt -w 800 test.out.4 > test.out.3
test $? -eq 0 || no_result

sed "s@$testdir/@@g" test.out.3 > test.out.2
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

# vim: set ts=8 sw=4 et :
