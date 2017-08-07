#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2009 Peter Miller
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

TEST_SUBJECT="open vs EEXIST"
. test_prelude

fmt > test.ok << fubar
open(pathname = "foo/bar/baz", flags = O_WRONLY | O_CREAT | O_EXCL, mode =
S_IRUSR | S_IRGRP | S_IROTH) failed, File exists (EEXIST) because there
is already a "baz" regular file in the "$testdir/guano" directory
fubar
test $? -eq 0 || no_result

mkdir foo
test $? -eq 0 || no_result

mkdir -p guano
test $? -eq 0 || no_result

ln -s $testdir/guano foo/bar
test $? -eq 0 || no_result

date > foo/bar/baz
test $? -eq 0 || no_result

test_open -f O_WRONLY+O_CREAT+O_EXCL -m 0444 foo/bar/baz > test.log 2>&1
if test $? -ne 1
then
    echo should have failed
    cat test.log
    fail
fi

fmt < test.log > test.out
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
