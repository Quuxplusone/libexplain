#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2008 Peter Miller
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

TEST_SUBJECT="execve EINVAL"
. test_prelude

cat > prog << 'fubar'
#!/bin/sh
exit 0
fubar
test $? -eq 0 || no_result

chmod a+rx prog
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
execve(pathname = "./prog", argv = ["./prog"], envp = [/* vars */]) failed,
Invalid argument (EINVAL) because an ELF executable had more than one
PT_INTERP segment (tried to name more than one interpreter); or, the new
process image file has the appropriate permission and has a recognized
executable binary format, but the system does not support execution of a
file with this format
fubar
test $? -eq 0 || no_result

explain -eEINVAL execve ./prog > test.out
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
