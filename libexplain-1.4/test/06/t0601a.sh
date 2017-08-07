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

TEST_SUBJECT="ioctl VIDIOC_LOG_STATUS"
. test_prelude

test_config HAVE_LINUX_VIDEODEV2_H || pass

cat > test.ok << 'fubar'
ioctl(fildes = 42, request = VIDIOC_LOG_STATUS, data = 0) failed, Invalid
argument (EINVAL) because fildes is not associated with an object to which
ioctl VIDIOC_LOG_STATUS can be applied
fubar
test $? -eq 0 || no_result

explain -e EINVAL ioctl 42 VIDIOC_LOG_STATUS 0 > test.out
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
