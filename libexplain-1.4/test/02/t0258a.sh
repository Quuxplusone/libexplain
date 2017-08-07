#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2008, 2011 Peter Miller
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
system(command = "kill -2 $$"): success, but the child process was
terminated by the Interrupt signal
fubar
test $? -eq 0 || no_result

# some very old shells exit differently in this case
cat > test.ok.2 << 'fubar'
system(command = "kill -2 $$"): success, but the child process terminated
with exit status 208
fubar
test $? -eq 0 || no_result

test_system 'kill -2 $$' > test.out 2>&1
if test $? -ne 1
then
    echo expected to fail
    cat test.out
    fail
fi

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
