#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2008, 2012 Peter Miller
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

TEST_SUBJECT="open vs ELOOP"
. test_prelude

# see if /proc is useful.
# it prints the explanation if it fails, causing a pass-by-default
test_getppcwd_works || pass

cat > test.ok << 'fubar'
open(pathname = "a", flags = O_RDONLY) failed, Too many levels of symbolic
links (ELOOP) because a symbolic link loop was encountered in pathname,
starting at "a"
fubar
test $? -eq 0 || no_result

ln -s a b
test $? -eq 0 || no_result

ln -s b c
test $? -eq 0 || no_result

ln -s c a
test $? -eq 0 || no_result

test_open -f O_RDONLY a > test.out 2>&1
if test $? -ne 1
then
    echo "meant to fail"
    cat test.out
    fail
fi

diff test.ok test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
