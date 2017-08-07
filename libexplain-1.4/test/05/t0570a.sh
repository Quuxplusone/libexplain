#!/bin/sh
#
# libexplain - a library of system-call-specific strerror replacements
# Copyright (C) 2011, 2012 Peter Miller
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

TEST_SUBJECT="ioctl VIDIOC_QBUF"
. test_prelude

test_config HAVE_LINUX_VIDEODEV2_H || pass

cat > test.ok.1 << 'fubar'
ioctl(fildes = 42, request = VIDIOC_QBUF, struct v4l2_buffer *data = {
index = 0, type = 0, bytesused = 0, flags = 0, field = V4L2_FIELD_ANY,
timestamp = { 0 seconds }, timecode = { type = 0, flags = 0, frames = 0,
seconds = 0, minutes = 0, hours = 0 }, sequence = 0, memory = 0, length =
0, input = 0 }) failed, Invalid argument (EINVAL) because the data->type
argument was incorrectly specified
fubar
test $? -eq 0 || no_result

cat > test.ok.2 << 'fubar'
ioctl(fildes = 42, request = VIDIOC_QBUF, struct v4l2_buffer *data = {
index = 0, type = 0, bytesused = 0, flags = 0, field = V4L2_FIELD_ANY,
timestamp = { 0 seconds }, timecode = { type = 0, flags = 0, frames = 0,
seconds = 0, minutes = 0, hours = 0 }, sequence = 0, memory = 0, length = 0
}) failed, Invalid argument (EINVAL) because the data->type argument was
incorrectly specified
fubar
test $? -eq 0 || no_result

explain -e EINVAL ioctl 42 VIDIOC_QBUF > test.out
test $? -eq 0 || fail

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
