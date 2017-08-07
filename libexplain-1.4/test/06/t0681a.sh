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

TEST_SUBJECT="setreuid EPERM"
. test_prelude

test_config not-root || pass

fmt > test.ok << 'fubar'
setreuid(ruid = 63419, euid = -1) failed, Operation not permitted (EPERM)
because the ruid argument does not match the real user ID or the effective
user ID or the saved user ID of the calling process, and the process is
not privileged
fubar
test $? -eq 0 || no_result

explain -e EPERM setreuid 63419 > test.out.4
test $? -eq 0 || fail

fmt -w 800 test.out.4 > test.out.3
test $? -eq 0 || no_result
sed 's|user ID ([^)]*)|user ID|g' test.out.3 > test.out.2
test $? -eq 0 || no_result
fmt test.out.2 > test.out
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
