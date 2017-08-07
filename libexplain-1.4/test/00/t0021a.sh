#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2008, 2010 Peter Miller
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

TEST_SUBJECT="fcntl vs ENOLCK"
. test_prelude

cat > test.ok << 'fubar'
fcntl(fildes = 42, command = F_SETLK, data = { l_type = F_RDLCK, l_whence =
SEEK_SET, l_start = 0, l_len = 0, l_pid = 0 }) failed, No locks available
(ENOLCK) because too many segment locks are open, or the lock table is
full, or a remote locking protocol failed (e.g. locking over NFS)
fubar
test $? -eq 0 || no_result

explain -e ENOLCK fcntl 42 F_SETLK > test.out
test $? -eq 0 || fail

diff test.ok test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim:ts=8:sw=4:et
