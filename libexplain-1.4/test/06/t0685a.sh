#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2012 Peter Miller
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

TEST_SUBJECT="chmod ENOENT"
. test_prelude

cat > test.ok.1 << 'fubar'
lchmod(pathname = "foobar", mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
failed, No such file or directory (ENOENT) because there is no "foobar"
regular file in the current directory
fubar
test $? -eq 0 || no_result

cat > test.ok.2 << 'fubar'
lchmod(pathname = "foobar", mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
failed, Function not implemented (ENOSYS) because this system does not
support the lchmod system call
fubar
test $? -eq 0 || no_result

test_lchmod foobar 0644 > test.out 2>&1
if test $? -ne 1
then
    echo expected to fail
    test -f test.out && cat test.out
    fail
fi

diff test.ok.2 test.out > /dev/null 2>/dev/null && pass

diff test.ok.1 test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
