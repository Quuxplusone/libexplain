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

TEST_SUBJECT="open ENOMEDIUM"
. test_prelude

if explain -eENOMEDIUM strerror >/dev/null 2>&1
then
    : ok
else
    echo
    echo "    Your system does not have the ENOMEDIUM error."
    echo "    This test is declared to pass by default."
    echo
    pass
fi

cat > test.ok << 'fubar'
open(pathname = "/dev/some/disk", flags = O_RDONLY) failed, No medium found
(ENOMEDIUM) because the disk drive is a type that has removable disks, and
there does not appear to be a disk in the drive
fubar
test $? -eq 0 || no_result

explain -eENOMEDIUM open /dev/some/disk > test.out
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
