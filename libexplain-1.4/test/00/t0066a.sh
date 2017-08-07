#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2008, 2010, 2011 Peter Miller
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

TEST_SUBJECT="lstat ELOOP"
. test_prelude

fmt > test.ok << 'fubar'
lstat(pathname = "a/b", data = xxxxxxxx) failed, Too many levels of
symbolic links (ELOOP) because a symbolic link loop was encountered
in pathname, starting at "a"
fubar
test $? -eq 0 || no_result

ln -s a b
test $? -eq 0 || no_result
ln -s b a
test $? -eq 0 || no_result

test_lstat a/b > test.out1 2>&1
if test $? -ne 1
then
    echo "meant to fail"
    cat test.out
    fail
fi

fmt -w 500 test.out1 > test.out2
test $? -eq 0 || no_result

sed 's|, data = [^)]*)|, data = xxxxxxxx)|' test.out2 > test.out3
test $? -eq 0 || no_result

fmt test.out3 > test.out
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
