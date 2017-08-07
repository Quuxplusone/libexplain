#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2010, 2011 Peter Miller
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

TEST_SUBJECT="vfprintf EINVAL"
. test_prelude

fmt > test.ok << 'fubar'

vfprintf(fp = 0xNNNNNNNN "/dev/null", format = NULL, ap = 0xNNNNNNNN) failed,
Invalid argument (EINVAL) because format is the NULL pointer

vfprintf(fp = 0xNNNNNNNN "/dev/null", format = "...%l%...", ap = 0xNNNNNNNN)
failed, Invalid argument (EINVAL) because within the format argument the
conversion specification "%l%", starting at position 3, is not valid

vfprintf(fp = 0xNNNNNNNN "/dev/null", format = "...%_...", ap = 0xNNNNNNNN)
failed, Invalid argument (EINVAL) because within the format argument the
conversion specification "%_", starting at position 3, is not valid

vfprintf(fp = 0xNNNNNNNN "/dev/null", format = "...%2$ 'I#0lzjqLh_...", ap
= 0xNNNNNNNN) failed, Invalid argument (EINVAL) because within the format
argument the conversion specification "%2$ 'I#0lz", starting at position 3,
is not valid

vfprintf(fp = 0xNNNNNNNN "/dev/null", format = "%1$d%3$d", ap = 0xNNNNNNNN)
failed, Invalid argument (EINVAL) because the format argument does not
contain a "%2$" specification
fubar
test $? -eq 0 || no_result

test_vfprintf > test.out4 2>&1
test $? -eq 0 || fail

fmt -w 700 < test.out4 > test.out3
test $? -eq 0 || no_result

sed -e "s|0x[0-9A-Fa-f][0-9A-Fa-f]*|0xNNNNNNNN|g" \
    < test.out3 > test.out2
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

# vim:ts=8:sw=4:et
