#!/bin/bash
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
set -x

tarball="$1";
echo "tarball='$tarball'"
if [ -z "$tarball" ]
then
    echo "Usage: $0 <tarball>"
    #exit 1
    tarball=$( aesub 'web-site/${project}.tar.gz' | sed 's|[.]|-|' )
fi

project_name=$( aesub '${project trunk_name}' )

DH_VERBOSE=1;
export DH_VERBOSE;
dvsn=$( aesub '${change debian-version}' );
echo "dvsn='$dvsn'";

vsn=$( aesub '${change version}' );
echo "vsn='$vsn'";

neutered_tarball="${project_name}-${dvsn}.orig.tar.gz";

#
# We fake the .orig.tar.gz file to not have a debian/ directory,
# by filtering it out with tardy(1).  This silences a PITA lintian(1)
# warning, for no real benefit.
#
rm -rf web-site/debian
mkdir -p web-site/debian
tardy -rp=1 \
        -exclude 'debian/*' \
        -p=${project_name}-${dvsn} \
        $tarball \
        web-site/debian/$neutered_tarball;

user=$( bin/test_user );
key=;
if [ $user = archives ]
then
    key=-k19CEC7D5
else
    key="-us -uc"
fi

unset GPG_AGENT_INFO

echo Options Indexes > web-site/debian/.htaccess


# build the actual debian package
mkdir -p web-site/debian;
tar xzf $tarball -C web-site/debian
(
  cd web-site/debian/$(basename $tarball .tar.gz)
  ls -lA
  dpkg-buildpackage -sgpg $key -sa
  test $? -ne 0 && exit 1

  codename=$( gawk -F= '/CODENAME/{print $2}' /etc/lsb-release )
  #debuild $key -sa -changes-option\=-DDistribution\=$codename
  #test $? -ne 0 && exit 1
)

# Run Lintian o fix errors.
# The SNR is pretty shabby.
(
  cd web-site/debian;
  lintian -iIv --pedantic *.changes
)

exit 0
# vim: set ts=8 sw=4 et :
