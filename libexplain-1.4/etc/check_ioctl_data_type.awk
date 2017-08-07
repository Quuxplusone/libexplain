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

/\/\* data_size \*\// {
    data_size = $0
    n = index(data_size, "/")
    if (n > 0)
        data_size = substr(data_size, 1, n - 1);
    data_size = gensub(/^ */, "", 1, data_size);
    data_size = gensub(/ *$/, "", 1, data_size);
    data_size = gensub(/,$/, "", 1, data_size);
    data_size = gensub(/sizeof\((.*)\).*/, "\\1", "g", data_size);
    #print FILENAME ": " FNR ": data_size = \"" data_size "\""
    data_type_count = 0
}
/\/\* data_type \*\// {
    data_type = $0
    n = index(data_type, "/")
    if (n > 0)
        data_type = substr(data_type, 1, n - 1);
    data_type = gensub(/^ */, "", 1, data_type);
    data_type = gensub(/ *$/, "", 1, data_type);
    data_type = gensub(/,$/, "", 1, data_type);
    data_type = gensub(/"(.*)"/, "\\1", "g", data_type);
    #print FILENAME ": " FNR ": data_type = \"" data_type "\""

    data_type_count++
    if (data_type_count > 1)
    {
        print FILENAME ": " FNR ": too many data_type lines"
        ++nerr
    }

    if (data_size == "NOT_A_POINTER")
    {
        if (data_type != "intptr_t")
        {
            print FILENAME ": " FNR ": " \
                "data_type should be 'intptr_t', not '" data_type "'"
            ++nerr
        }
    }
    else if (data_size == "VOID_STAR")
    {
        if (data_type != "void *")
        {
            print FILENAME ": " FNR ": " \
                "data_type should be 'void *', not '" data_type "'"
            ++nerr
        }
    }
    else if (data_size != "0")
    {
        dt = data_size;
        if (substr(dt, length(dt), 1) != "]")
            dt = dt " *";
        if (dt != data_type)
        {
            print FILENAME ": " FNR ": " \
                "data_type should be \"" dt "\", not \"" data_type "\""
            ++nerr
        }
    }
    else if (data_size == "0")
    {
        if (data_type != "0")
        {
            print FILENAME ": " FNR ": " \
                "data_type should be 0, not \"" data_type "\""
            ++nerr
        }
    }
}

END {
    if (nerr != 0)
        exit(1)
}

# vim: set ts=8 sw=4 et :
