#!/bin/sh
#
# libexplain - a library of system-call-specific strerror replacements
# Copyright (C) 2011 Peter Miller
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

TEST_SUBJECT="realpath ENAMETOOLONG"
. test_prelude

fmt > test.ok << 'fubar'
realpath(pathname = "fu/1234567890 1234567890 1234567890 1234567890
1234567890 1234567890 1234567890 1234567890 1234567890 1234567890
1234567890 1234567890 1234567890 1234567890 1234567890 1234567890
1234567890 1234567890 1234567890 1234567890 1234567890 1234567890
1234567890 1234567890 1234567890 1234567890 1234567890 1234567890
1234567890 1234567890 1234567890 1234567890/baz", resolved_pathname =
0xNNNNNNNN) failed, File name too long (ENAMETOOLONG) because pathname
"1234567890 1234567890 1234567890 1234567890 1234567890 1234567890
1234567890 1234567890 1234567890 1234567890 1234567890 1234567890
1234567890 1234567890 1234567890 1234567890 1234567890 1234567890
1234567890 1234567890 1234567890 1234567890 1234567890 1234567890
1234567890 1234567890 1234567890 1234567890 1234567890 1234567890
1234567890 1234567890" component is longer than the system limit (351
> 255)
fubar
test $? -eq 0 || no_result

mkdir fu
test $? -eq 0 || no_result

bar=1234567890
bar="$bar $bar"
bar="$bar $bar"
bar="$bar $bar"
bar="$bar $bar"
bar="$bar $bar"

explain -eENAMETOOLONG realpath "fu/$bar/baz" > test.out.4
test $? -eq 0 || fail

fmt -w 900 test.out.4 > test.out.3
test $? -eq 0 || no_result
sed 's|0x[0-9A-F]*|0xNNNNNNNN|' test.out.3 > test.out.2
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
