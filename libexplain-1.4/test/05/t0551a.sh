#!/bin/sh
#
# libexplain - a library of system-call-specific strerror replacements
# Copyright (C) 2010-2012 Peter Miller
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

TEST_SUBJECT="ptrace EFAULT"
. test_prelude

test_config __linux__ || pass

cat > test.ok << 'fubar'
ptrace(request = PT_GETSIGINFO, pid = 0, addr = NULL, data = NULL) failed,
Bad address (EFAULT) because data refers to memory that is outside the
process's accessible address space; this is more likely to be a software
error (a bug) than it is to be a user error
fubar
test $? -eq 0 || no_result

cat > test.ok.2 << 'fubar'
ptrace(request = PT_GETSIGINFO, pid = 0, addr = NULL, data = NULL) failed,
Bad address (EFAULT) because there was an attempt to read from or write to
an invalid area in the parent's or child's memory, probably because the
area wasn't mapped or accessible
fubar
test $? -eq 0 || no_result

explain -eEFAULT ptrace PT_GETSIGINFO 0 0 > test.out
test $? -eq 0 || fail

diff test.ok.2 test.out > /dev/null 2>&1 && pass

diff test.ok test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim:ts=8:sw=4:et
