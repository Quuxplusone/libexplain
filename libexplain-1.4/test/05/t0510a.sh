#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2009, 2012 Peter Miller
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

TEST_SUBJECT="execlp EACCES"
. test_prelude

# this test doesn't work for root (or fakeroot)
test_config not-root || pass

cat > test.ok << 'fubar'
execlp(pathname = "cant-exec-me", arg = "cant-exec-me", "arg1", "arg2",
NULL) failed, Permission denied (EACCES) because the process does not have
execute permission to the "cant-exec-me" regular file in the current
directory
fubar
test $? -eq 0 || no_result

touch cant-exec-me

PATH=.:$PATH \
test_execlp cant-exec-me arg1 arg2 > test.out 2>&1
if test $? -ne 1
then
    cat test.out
    fail
fi

diff test.ok test.out
if test $? -ne 0
then
    cat test.out
    fail
fi

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
