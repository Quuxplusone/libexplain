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

TEST_SUBJECT="explain_open ELOOP"
. test_prelude

# Solaris is still in the Dark Ages
test_config O_NOFOLLOW || pass

touch snot
test $? -eq 0 || no_result
ln -s snot slink
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
open(pathname = "slink", flags = O_RDONLY | O_NOFOLLOW) failed, Too many
levels of symbolic links (ELOOP) because O_NOFOLLOW was specified but
pathname refers to a symbolic link
fubar
test $? -eq 0 || no_result

# This is for FreeBSD
cat > test.ok2 << 'fubar'
open(pathname = "slink", flags = O_RDONLY | O_NOFOLLOW) failed, Too many
links (EMLINK) because O_NOFOLLOW was specified but pathname refers to a
symbolic link
fubar
test $? -eq 0 || no_result

test_open -f'O_RDONLY+O_NOFOLLOW' slink > test.out 2>&1
if test $? -ne 1
then
    echo "it should fail"
    cat test.out
    fail
fi

diff test.ok test.out >/dev/null 2>&1 && pass

diff test.ok2 test.out
test $? -eq 0 || fail

#
#
# Only definite negatives are possible.
# The functionality exercised by this test appears to work,
# no other guarantees are made.
#
pass

# vim:ts=8:sw=4:et
