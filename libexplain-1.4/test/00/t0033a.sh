#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2008-2011 Peter Miller
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

TEST_SUBJECT="write vs ENOSPC"
. test_prelude

fmt > test.ok.1 << 'fubar'
write(fildes = 1, data = 0x09876543, data_size = 1110) failed, No
space left on device (ENOSPC) because the file system containing fildes
("/example", 99% full) has no more space for data
fubar
test $? -eq 0 || no_result

fmt > test.ok.2 << 'fubar'
write(fildes = 1, data = 0x09876543, data_size = 1110) failed, No
space left on device (ENOSPC) because the file system containing fildes
("/example", 99% full) has no more space for a new directory entry
fubar
test $? -eq 0 || no_result

fmt > test.ok.3 << 'fubar'
write(fildes = 1, data = 0x09876543, data_size = 1110) failed, No space
left on device (ENOSPC) because the file system containing fildes has
no more space for data
fubar
test $? -eq 0 || no_result

fmt > test.ok.4 << 'fubar'
write(fildes = 1, data = 0x09876543, data_size = 1110) failed, No space
left on device (ENOSPC) because the file system containing fildes has
no more space for a new directory entry
fubar
test $? -eq 0 || no_result

explain -e ENOSPC write 1 0x9876543 0x456 > test.out.narrow
test $? -eq 0 || fail

fmt -w 300 test.out.narrow > test.out.wide
test $? -eq 0 || no_result

sed -e 's|= 1 "[^"]*",|= 1,|' \
    -e 's|(".*", .*% full)|("/example", 99% full)|' \
        test.out.wide > test.out.cooked
test $? -eq 0 || no_result

fmt test.out.cooked > test.out
test $? -eq 0 || no_result

diff test.ok.1 test.out > /dev/null 2>/dev/null && pass
diff test.ok.2 test.out > /dev/null 2>/dev/null && pass
diff test.ok.3 test.out > /dev/null 2>/dev/null && pass

diff test.ok.4 test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim:ts=8:sw=4:et
