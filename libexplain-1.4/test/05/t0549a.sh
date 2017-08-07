#!/bin/sh
#
# libexplain - a library of system-call-specific strerror replacements
# Copyright (C) 2010 Peter Miller
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

TEST_SUBJECT="hanging-indent=8"
. test_prelude

cat > test.ok << 'fubar'
open(pathname = "aa bb cc dd ee ff gg hh ii jj kk ll mm nn oo pp qq rr ss
        tt uu vv ww xx yy zz", flags = O_RDONLY) failed, No such file or
        directory (2, ENOENT) because there is no "aa bb cc dd ee ff gg hh
        ii jj kk ll mm nn oo pp qq rr ss tt uu vv ww xx yy zz" regular file
        in the current directory
fubar
test $? -eq 0 || no_result

EXPLAIN_OPTIONS='hanging-indent=8' \
explain -eENOENT open "aa bb cc dd ee ff gg hh ii jj kk ll mm nn oo pp qq \
rr ss tt uu vv ww xx yy zz" > test.out
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
