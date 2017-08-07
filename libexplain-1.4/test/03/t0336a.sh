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

TEST_SUBJECT="lchown EPERM"
. test_prelude

# this test doesn't work for root (or fakeroot)
test_config not-root || pass

fmt > test.ok << 'fubar'
lchown(pathname = "foobar", owner = 0, group = 0) failed, Operation
not permitted (EPERM) because the process effective UID nnn is the same
as the owner UID of pathname but this is not sufficient privilege to
change the owner UID, and the process is not privileged
fubar

touch foobar
test $? -eq 0 || no_result

explain -eEPERM lchown foobar 0 0 > test.out4
test $? -eq 0 || fail

fmt -w 700 < test.out4 > test.out3
test $? -eq 0 || no_result

sed -e 's|group = 0 "[^"]*"|group = 0|' \
    -e 's|UID [0-9][0-9]* *"[^"]*"|UID nnn|' \
    -e 's|UID [0-9][0-9]*|UID nnn|' \
    test.out3 > test.out2
test $? -eq 0 || no_result

fmt < test.out2 > test.out
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
