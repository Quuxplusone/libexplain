#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2009, 2010 Peter Miller
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

TEST_SUBJECT="flock ENOLCK"
. test_prelude

if test `uname -s` = "SunOS"
then
    echo
    echo "    Solaris does not have the flock system call."
    echo "    This test is declared to pass by default."
    echo
    pass
fi

cat > test.ok << 'fubar'
flock(fildes = 42, command = LOCK_EX) failed, No locks available (ENOLCK)
because insufficient kernel memory was available
fubar
test $? -eq 0 || no_result

explain -eENOLCK flock 42 LOCK_EX > test.out
test $? -eq 0 || fail

diff test.ok test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim:ts=8:sw=4:et
