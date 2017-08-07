#
# libexplain - Explain errno values returned by libc functions
# Copyright (C) 2009-2011, 2013 Peter Miller
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
#
#     This script is used to reproduce a subset of the config.h file,
#     the parts that apply to large files, using a LIBEXPLAIN name
#     prefix, to avoid collisions with the user's program.
BEGIN {
    print "#ifndef LIBEXPLAIN_PUBLIC_CONFIG_H"
    print "#define LIBEXPLAIN_PUBLIC_CONFIG_H"
    print ""
}
/LIBEXPLAIN_/{
    next
}

{
    line = $0
    n = index(line, "#define ");
    if (n > 0)
    {
        n += 8
        lhs = substr(line, 1, n - 1)
        rhs = substr(line, n, 999)
        line = lhs "LIBEXPLAIN_" rhs
    }
    else
    {
        n = index(line, "#undef ");
        if (n > 0)
        {
            n += 7
            lhs = substr(line, 1, n - 1)
            rhs = substr(line, n, 999)
            line = lhs "LIBEXPLAIN_" rhs
        }
    }
    print line
}
END {
    print ""
    print "#endif /* LIBEXPLAIN_PUBLIC_CONFIG_H */"
}


# vim: set ts=8 sw=4 et :
