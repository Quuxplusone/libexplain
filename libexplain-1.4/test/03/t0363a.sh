#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2009, 2011 Peter Miller
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

TEST_SUBJECT="execvp ENOENT"
. test_prelude

fmt > test.ok << 'fubar'
execvp(pathname = "no-such-command", argv = ["no-such-command"]) failed,
No such file or directory (ENOENT) because there is no "no-such-command"
regular file in any of the command search PATH directories (...)
fubar
test $? -eq 0 || no_result

test_execvp no-such-command > test.out5 2>&1
if test $? -ne 1
then
    cat test.out5
    fail
fi

fmt -w 700 test.out5 > test.out4
test $? -eq 0 || no_result

head -1 test.out4 > test.out3
test $? -eq 0 || no_result

sed 's|PATH directories.*|PATH directories (...)|' test.out3 > test.out2
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
