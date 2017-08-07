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

TEST_SUBJECT="access EACCES"
. test_prelude

# this test doesn't work for root (or fakeroot)
test_config not-root || pass

cat > test.ok << 'fubar'
access(pathname = "foobar", mode = W_OK | R_OK) failed, Permission denied
(EACCES) because the process does not have read permission to the "foobar"
regular file in the current directory; note that it is an error if any of
the access types in mode are denied, even if some of the other access types
in mode would be permitted
fubar
test $? -eq 0 || no_result

touch foobar
test $? -eq 0 || no_result

chmod a-r foobar
test $? -eq 0 || no_result

test_access -m R_OK+W_OK foobar > test.out 2>&1
if test $? -ne 1
then
    echo expected to fail
    test -f test.out && cat test.out
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
