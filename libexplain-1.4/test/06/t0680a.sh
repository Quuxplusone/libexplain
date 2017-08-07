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

TEST_SUBJECT="explain_filename_from_fildes"
. test_prelude

# This test gives variable results on FreeBSD
if [ `uname -s` = FreeBSD ]
then
    echo "    This test is declared to pass by default"
    pass
fi


cat > test.in << 'fubar'
Hello, World!
fubar
test $? -eq 0 || no_result

cat > test.ok << fubar
stdin = $testdir/test.in
stdout = $testdir/test.out
fubar
test $? -eq 0 || no_result

test_filename < test.in > test.out
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
