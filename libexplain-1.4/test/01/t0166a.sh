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

TEST_SUBJECT="rmdir EACCES"
. test_prelude

# this test doesn't work for root (or fakeroot)
test_config not-root || pass

cat > test.ok << 'fubar'
rmdir(pathname = "a/b/c") failed, Permission denied (EACCES) because the
process does not have write permission to the pathname "a/b" directory,
this is needed to remove the directory entry for the "c" directory; note
that pathname still exists
fubar
test $? -eq 0 || no_result

mkdir -p a/b/c
test $? -eq 0 || no_result

chmod a-w a/b
test $? -eq 0 || no_result

test_rmdir a/b/c > test.out 2>&1
if test $? -ne 1
then
    chmod u+w a/b
    echo expected to fail
    test -f test.out && cat test.out
    fail
fi

chmod u+w a/b

diff test.ok test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
