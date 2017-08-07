#!/bin/sh
#
# libexplain - a library of system-call-specific strerror replacements
# Copyright (C) 2013 Peter Miller
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

TEST_SUBJECT="acl_get_file"
. test_prelude

cat > test.ok << 'fubar'
acl_get_file(pathname = "test.in", type = ACL_TYPE_ACCESS) failed, No such
file or directory (ENOENT) because there is no "test.in" regular file in
the current directory; did you mean the "test.ok" regular file instead?
fubar
test $? -eq 0 || no_result

explain -eENOENT acl_get_file test.in ACL_TYPE_ACCESS > test.out
test $? -eq 0 || fail

diff test.ok test.out
test $? -eq 0 || fail

#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
