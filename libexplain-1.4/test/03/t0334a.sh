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

TEST_SUBJECT="fpathconf EINVAL"
. test_prelude

fmt -w 75 > test.ok << 'fubar'
fpathconf(fildes = 1, name = 666) failed, Invalid argument (EINVAL)
because name does not refer to a known file configuration value
fubar
test $? -eq 0 || no_result

test_fpathconf 1 666 > test.out4 2>&1
if test $? -ne 1
then
    echo expected to fail
    cat test.out
    fail
fi

fmt -w 500 < test.out4 > test.out3
test $? -eq 0 || no_result

sed -e 's|fildes = 1[^,]*|fildes = 1|' < test.out3 > test.out2
test $? -eq 0 || no_result

fmt -w 75 < test.out2 > test.out
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
