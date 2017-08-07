#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2008, 2010-2012 Peter Miller
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

TEST_SUBJECT="readlink EACCES"
. test_prelude

# this test doesn't work for root (or fakeroot)
test_config not-root || pass

fmt > test.ok << 'fubar'
readlink(pathname = "foo/bar", data = 0xNNNNNNNN, data_size = N)
failed, Permission denied (EACCES) because the process does not have
search permission to the pathname "foo" directory
fubar
test $? -eq 0 || no_result

mkdir foo
test $? -eq 0 || no_result

chmod a-rwx foo
test $? -eq 0 || no_result

test_readlink foo/bar > test.out4 2>&1
if test $? -ne 1
then
    chmod u+rwx foo
    echo expected to fail
    cat test.out
    fail
fi

chmod u+rwx foo

fmt -w 700 test.out4 > test.out3
test $? -eq 0 || no_result

sed -e 's|data = [^,]*|data = 0xNNNNNNNN|' \
    -e 's|data_size = [0-9]*|data_size = N|' \
    test.out3 > test.out2
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

# vim: set ts=8 sw=4 et :
