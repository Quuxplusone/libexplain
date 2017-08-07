#!/bin/sh
#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2009-2011 Peter Miller
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

TEST_SUBJECT="ioctl MTIOCSETCONFIG"
. test_prelude

sys=`uname -s`
case "$sys" in
SunOS | FreeBSD)
    echo
    echo "      $sys does not have ioctl MTIOCSETCONFIG."
    echo "      This test is declared to pass by default."
    echo
    pass
    ;;
*)
    ;;
esac

cat > test.ok << 'fubar'
ioctl(fildes = 42, request = MTIOCSETCONFIG, struct mtconfiginfo *data = {
mt_type = 0, ifc_type = 0, irqnr = 0, dmanr = 0, port = 0, debug = 0,
have_dens = 0, have_bsf = 0, have_fsr = 0, have_bsr = 0, have_eod = 0,
have_seek = 0, have_tell = 0, have_ras1 = 0, have_ras2 = 0, have_ras3 = 0,
have_qfa = 0, }) failed, Inappropriate ioctl for device (ENOTTY) because
fildes is not associated with an object to which ioctl MTIOCSETCONFIG can
be applied
fubar
test $? -eq 0 || no_result

explain -eENOTTY ioctl 42 MTIOCSETCONFIG > test.out
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
