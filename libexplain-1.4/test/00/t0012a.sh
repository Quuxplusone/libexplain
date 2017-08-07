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

TEST_SUBJECT="write vs EPIPE"
. test_prelude

cat > test.ok << 'fubar'
write(fildes = 42, data = 0x00001000, data_size = 10) failed, Broken pipe
(EPIPE) because the file descriptor is connected to a pipe or socket whose
reading end is closed, when this happens the writing process will also
receive a SIGPIPE signal; note that this process catches, blocks or ignores
the SIGPIPE signal
fubar
test $? -eq 0 || no_result

explain -e EPIPE write 42 0x1000 10 > test.out
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
