#!/bin/sh
#
# libexplain - a library of system-call-specific strerror replacements
# Copyright (C) 2010 Peter Miller
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

TEST_SUBJECT="munmap EINVAL"
. test_prelude

cat > test.ok << 'fubar'
munmap(data = 0x00040001, data_size = 4096) failed, Invalid argument
(EINVAL) because the data must be a multiple of the page size (NNN)
fubar
test $? -eq 0 || no_result

explain -eEINVAL munmap 0x40001 0x1000 > test.out2
test $? -eq 0 || fail

sed 's|([1-9][0-9]*)|(NNN)|g' test.out2 > test.out
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
