#!/bin/sh
#
# libexplain - a library of system-call-specific strerror replacements
# Copyright (C) 2013 Peter Miller
# Written by Peter Miller <pmiller@opensource.org.au>
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 3 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along with
# this program. If not, see <http://www.gnu.org/licenses/>.
#

TEST_SUBJECT="mount vs EINVAL"
. test_prelude

cat > test.ok << 'fubar'
mount(source = "gordon", target = "percy", file_system_type = "ext3x",
flags = 0, data = "here,is,some,data") failed, Invalid argument (EINVAL)
because source had an invalid superblock
fubar
test $? -eq 0 || no_result

touch gordon
test $? -eq 0 || no_result
mkdir percy
test $? -eq 0 || no_result

explain -eEINVAL mount gordon percy ext3x 0 \
    "here,is,some,data" > test.out

diff test.ok test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
