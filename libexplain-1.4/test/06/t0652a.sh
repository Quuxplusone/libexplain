#!/bin/sh
#
# libexplain - a library of system-call-specific strerror replacements
# Copyright (C) 2011 Peter Miller
# Written by Peter Miller <pmiller@opensource.org.au>
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 3 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along with
# this program. If not, see <http://www.gnu.org/licenses/>.
#

TEST_SUBJECT='fputs("yuck", stdin);'
. test_prelude

# writing on stdin will usually barf, as this test case expectes,
# even if the underlying file descriptor was r/w before calling dup(2).
fmt > test.ok.1 << 'fubar'
fputs(s = "yuck", fp = stdin) failed, Bad file descriptor (EBADF)
because the fp argument does not refer to an object that is open for
writing; this is more likely to be a software error (a bug) than it is to
be a user error
fubar
test $? -eq 0 || no_result

# There is an alternative result, depending on the testing enviroment,
# because stdin may actually be closed.
fmt > test.ok.2 << 'fubar'
fputs(s = "yuck", fp = stdin) failed, Bad file descriptor (EBADF)
because fildes does not refer to an open file; this is more likely to
be a software error (a bug) than it is to be a user error
fubar
test $? -eq 0 || no_result

test_fputs yuck stdin > test.out.4 2>&1
if test $? -ne 1
then
    set +x
    echo "it was supposed to fail" 1>&2
    cat test.out.4
    fail
fi

fmt -w 800  test.out.4 > test.out.3
test $? -eq 0 || no_result
sed 's/stdin "[^"]*")/stdin)/' test.out.3 > test.out.2
test $? -eq 0 || no_result
fmt test.out.2 > test.out
test $? -eq 0 || no_result

diff test.ok.2 test.out > /dev/null 2>&1 && pass

diff test.ok.1 test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
