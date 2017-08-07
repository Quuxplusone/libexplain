#!/bin/sh
#
# libexplain - a library of system-call-specific strerror replacements
# Copyright (C) 2011, 2012 Peter Miller
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

TEST_SUBJECT="shmat EACCES"
. test_prelude

# this test doesn't work for root (or fakeroot)
test_config not-root || pass

cat > test.ok << 'fubar'
shmat(shmid = 54321, shmaddr = NULL, shmflg = 0) failed, Permission denied
(EACCES) because the process does not have the necessary shared memory
segment access permissions, and the process is not privileged
fubar
test $? -eq 0 || no_result

explain -eEACCES shmat 54321 0 0 > test.out
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
