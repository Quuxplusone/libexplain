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

TEST_SUBJECT="mkdir EACCES"
. test_prelude

# this test doesn't work for root (or fakeroot)
test_config not-root || pass

cat > test.ok << 'fubar'
mkdir(pathname = "a/foo/bar", mode = S_IRWXU | S_IRWXG | S_IRWXO) failed,
Permission denied (EACCES) because the process does not have write
permission to the pathname "a/foo" directory, this is needed to create the
directory entry for the "bar" directory
fubar
test $? -eq 0 || no_result

mkdir -p a/foo
test $? -eq 0 || no_result

chmod a-w a/foo
test $? -eq 0 || no_result

test_mkdir a/foo/bar > test.out 2>&1
if test $? -ne 1
then
    chmod u+w a/foo
    echo expected to fail
    test -f test.out && cat test.out
    fail
fi

chmod u+w a/foo

diff test.ok test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
