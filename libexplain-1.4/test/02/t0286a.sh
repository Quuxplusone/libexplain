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

TEST_SUBJECT="connect EFAULT"
. test_prelude

fmt > test.ok << 'fubar'
connect(fildes = 42, serv_addr = 0x09876543, serv_addr_size = NNN) failed,
Bad address (EFAULT) because sock_addr refers to memory that is outside
the process's accessible address space; this is more likely to be a
software error (a bug) than it is to be a user error
fubar
test $? -eq 0 || no_result

explain -eEFAULT connect 42 0x9876543 100 > test.out4
test $? -eq 0 || fail

fmt -w 700 test.out4 > test.out3
test $? -eq 0 || no_result

sed 's|serv_addr_size = [1-9][0-9]*|serv_addr_size = NNN|' test.out3 > test.out2
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

# vim:ts=8:sw=4:et
