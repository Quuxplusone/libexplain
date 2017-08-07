#!/bin/sh
#
# libexplain - a library of system-call-specific strerror replacements
# Copyright (C) 2012 Peter Miller
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

TEST_SUBJECT="getresuid EFAULT"
. test_prelude

cat > test.ok << 'fubar'
getresuid(ruid = 0x00000001, euid = 0x00000002, suid = 0x00000003) failed,
Bad address (EFAULT) because ruid refers to memory that is outside the
process's accessible address space; this is more likely to be a software
error (a bug) than it is to be a user error
fubar
test $? -eq 0 || no_result

explain -e EFAULT getresuid 1 2 3 > test.out
test $? -eq 0 || fail

diff test.ok test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
