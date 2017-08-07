#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2008, 2009, 2011 Peter Miller
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

TEST_SUBJECT="system"
. test_prelude

cat > test.ok.1 << 'fubar'
system(command = "foobar baz"): success, but the child process terminated
with exit status 127, "foobar" command not found on $PATH
fubar
test $? -eq 0 || no_result

cat > test.ok.2 << 'fubar'
system(command = "foobar baz"): success, but the child process terminated
with exit status EXIT_FAILURE (1)
fubar
test $? -eq 0 || no_result

test_system 'foobar baz' > test.out2 2>&1
if test $? -ne 1
then
    echo expected to fail
    cat test.out
    fail
fi

# Rip off the first line.
# Different shells produce different text.
sed '1d' test.out2 > test.out
test $? -eq 0 || no_result

diff test.ok.2 test.out > /dev/null 2>&1  && pass

diff test.ok.1 test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim:ts=8:sw=4:et
