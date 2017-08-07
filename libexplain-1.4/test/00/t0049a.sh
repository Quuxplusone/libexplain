#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2008, 2010 Peter Miller
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

TEST_SUBJECT="read EIO"
. test_prelude

cat > test.ok << 'fubar'
read(fildes = 42, data = 0x00000100, data_size = 100) failed, Input/output
error (EIO) because a low-level I/O error occurred, probably in hardware,
possibly as a result of a preceding read(2) or write(2) system call
fubar
test $? -eq 0 || no_result

# This is what the Debian automated build system (FTBFS) bumps into.
cat > test.ok2 << 'fubar'
read(fildes = 42, data = 0x00000100, data_size = 100) failed, Input/output
error (EIO) because the process is in an orphaned process group and tried
to read from its controlling tty
fubar
test $? -eq 0 || no_result

explain -e EIO -o test.out read 42 0x100 100
test $? -eq 0 || fail

diff test.ok2 test.out > LOG 2>&1
if [ $? -ne 0 ]
then
    diff test.ok test.out > LOG 2>&1
    if [ $? -ne 0 ]
    then
        cat LOG
        fail
    fi
fi

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim:ts=8:sw=4:et
