#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2009-2012 Peter Miller
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

TEST_SUBJECT="execvp EACCES"
. test_prelude

# this test doesn't work for root (or fakeroot)
test_config not-root || pass

fmt > test.ok << 'fubar'
execvp(pathname = "yuck", argv = ["yuck"]) failed, Permission denied
(EACCES) because the process does not have execute permission to the
"yuck" regular file in the pathname "ugly" directory
fubar
test $? -eq 0 || no_result

mkdir bogus ugly
test $? -eq 0 || no_result

date > ugly/yuck
test $? -eq 0 || no_result

chmod a-x ugly/yuck
test $? -eq 0 || no_result

PATH=`pwd`/bogus:`pwd`/ugly:$PATH
export PATH

test_execvp yuck > test.out4 2>&1
if test $? -ne 1
then
    cat test.out5
    fail
fi

fmt -w 700 test.out4 > test.out3
test $? -eq 0 || no_result

sed 's|"[^"]*/ugly" directory|"ugly" directory|' test.out3 > test.out2
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

# vim: set ts=8 sw=4 et :
