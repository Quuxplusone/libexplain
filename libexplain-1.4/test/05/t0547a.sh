#!/bin/sh
#
# libexplain - a library of system-call-specific strerror replacements
# Copyright (C) 2010, 2011 Peter Miller
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

TEST_SUBJECT="calloc ENOMEM"
. test_prelude

fmt > test.ok << 'fubar'
calloc(nmemb = 7, size = 6) failed, Cannot allocate memory (ENOMEM)
because insufficient user-space memory was available, probably by
exhausting swap space
fubar
test $? -eq 0 || no_result

fmt > test.ok2 << 'fubar'
calloc(nmemb = 7, size = 6) failed, Cannot allocate memory (ENOMEM)
because insufficient user-space memory was available { rlim_cur = NNN }
fubar
test $? -eq 0 || no_result

explain -e ENOMEM calloc 7 6 > test.out.4
test $? -eq 0 || fail

fmt -w 700 test.out.4 > test.out.3
test $? -eq 0 || no_result
sed 's|{ rlim_cur = [0-9]* }|{ rlim_cur = NNN }|' test.out.3 > test.out.2
test $? -eq 0 || no_result
fmt test.out.2 > test.out
test $? -eq 0 || no_result

diff test.ok test.out >/dev/null 2>&1 && pass

diff test.ok2 test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim:ts=8:sw=4:et
