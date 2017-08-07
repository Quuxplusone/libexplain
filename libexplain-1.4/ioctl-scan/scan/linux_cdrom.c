/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2009, 2010 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,but
 * WITHOUT ANY WARRANTY; without even the implied warranty
 * ofMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNULesser
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <libexplain/ac/limits.h>
#include <libexplain/ac/linux/cdrom.h>
#include <libexplain/ac/stdio.h>
#include <libexplain/ac/sys/ioctl.h>

#include <libexplain/buffer/hexdump.h>
#include <libexplain/parse_bits.h>
#include <libexplain/sizeof.h>
#include <libexplain/string_buffer.h>
#include <libexplain/ioctl.h>

#include <ioctl-scan/report.h>
#include <ioctl-scan/scan/linux_cdrom.h>


#ifdef CDROMREADTOCHDR

static void
cdromreadtochdr(int fildes)
{
    struct cdrom_tochdr data;

    data.cdth_trk0 = 0;
    data.cdth_trk1 = 99;
    if (ioctl(fildes, CDROMREADTOCHDR, &data) >= 0)
    {
        report_uint64("CDROMREADTOCHDR", "First Track", data.cdth_trk0, 0);
        report_uint64("CDROMREADTOCHDR", "Last Track", data.cdth_trk1, 0);
    }
}

#endif

#ifdef CDROMREADTOCENTRY

static void
cdromreadtocentry(int fildes)
{
    struct cdrom_tochdr header;
    int             j;

    header.cdth_trk0 = 0;
    header.cdth_trk1 = 99;
    if (ioctl(fildes, CDROMREADTOCHDR, &header) < 0)
        return;
    for (j = header.cdth_trk0; j <= header.cdth_trk1; ++j)
    {
        struct cdrom_tocentry entry;

        entry.cdte_track = j;
        entry.cdte_format = CDROM_LBA;
        fflush(stdout);
        if (explain_ioctl_on_error(fildes, CDROMREADTOCENTRY, &entry) >= 0)
        {
            report_int("CDROMREADTOCENTRY", "Track", entry.cdte_track, 0);
            report_int("CDROMREADTOCENTRY", "    Adr", entry.cdte_adr, 0);
            report_int("CDROMREADTOCENTRY", "    Ctrl", entry.cdte_ctrl, 0);
            switch (entry.cdte_format)
            {
            case CDROM_MSF:
                {
                    char            buf[100];

                    snprintf
                    (
                        buf,
                        sizeof(buf),
                        " %d, %d, %d",
                        entry.cdte_addr.msf.minute,
                        entry.cdte_addr.msf.second,
                        entry.cdte_addr.msf.frame
                    );
                    report("CDROMREADTOCENTRY", "    Address", buf);
                }
                break;

            case CDROM_LBA:
                report_int
                (
                    "CDROMREADTOCENTRY",
                    "    Address",
                    entry.cdte_addr.lba,
                    0
                );
                break;

            default:
                report_int
                (
                    "CDROMREADTOCENTRY",
                    "    Format",
                    entry.cdte_format,
                    0
                );
                break;
            }
            report_int
            (
                "CDROMREADTOCENTRY",
                "    Data Mode",
                entry.cdte_datamode,
                0
            );
        }
    }
}

#endif

#ifdef CDROM_DRIVE_STATUS

static void
cdrom_drive_status(int fildes)
{
    int             status;

    status = ioctl(fildes, CDROM_DRIVE_STATUS, CDSL_CURRENT);
    if (status >= 0)
    {
        static const explain_parse_bits_table_t table[] =
        {
            { "CDS_NO_INFO", CDS_NO_INFO },
            { "CDS_NO_DISC", CDS_NO_DISC },
            { "CDS_TRAY_OPEN", CDS_TRAY_OPEN },
            { "CDS_DRIVE_NOT_READY", CDS_DRIVE_NOT_READY },
            { "CDS_DISC_OK", CDS_DISC_OK },
        };

        explain_string_buffer_t buf;
        char            text[30];

        explain_string_buffer_init(&buf, text, sizeof(text));
        explain_parse_bits_print_single(&buf, status, table, SIZEOF(table));
        report("CDROM_DRIVE_STATUS", "Drive status", text);
    }
}

#endif

#ifdef CDROM_DISC_STATUS

static void
cdrom_disc_status(int fildes)
{
    int             status;

    status = ioctl(fildes, CDROM_DISC_STATUS, 0);
    if (status >= 0)
    {
        static const explain_parse_bits_table_t table[] =
        {
            { "CDS_NO_INFO", CDS_NO_INFO },
            { "CDS_NO_DISC", CDS_NO_DISC },
            { "CDS_AUDIO", CDS_AUDIO },
            { "CDS_DATA_1", CDS_DATA_1 },
            { "CDS_DATA_2", CDS_DATA_2 },
            { "CDS_XA_2_1", CDS_XA_2_1 },
            { "CDS_XA_2_2", CDS_XA_2_2 },
            { "CDS_MIXED", CDS_MIXED },
        };

        explain_string_buffer_t buf;
        char            text[30];

        explain_string_buffer_init(&buf, text, sizeof(text));
        explain_parse_bits_print_single(&buf, status, table, SIZEOF(table));
        report("CDROM_DISC_STATUS", "Disc status", text);
    }
}

#endif

#ifdef CDROM_CHANGER_NSLOTS

static void
cdrom_changer_nslots(int fildes)
{
    int             capacity;

    capacity = ioctl(fildes, CDROM_CHANGER_NSLOTS, 0);
    if (capacity >= 0)
    {
        report_int("CDROM_CHANGER_NSLOTS", "Changer slots", capacity, 0);
    }
}

#endif

#ifdef CDROM_GET_CAPABILITY

static void
cdrom_get_capability(int fildes)
{
    int             capability;

    capability = ioctl(fildes, CDROM_GET_CAPABILITY, 0);
    if (capability >= 0)
    {
        static const explain_parse_bits_table_t table[] =
        {
            { "CDC_CLOSE_TRAY", CDC_CLOSE_TRAY },
            { "CDC_OPEN_TRAY", CDC_OPEN_TRAY },
            { "CDC_LOCK", CDC_LOCK },
            { "CDC_SELECT_SPEED", CDC_SELECT_SPEED },
            { "CDC_SELECT_DISC", CDC_SELECT_DISC },
            { "CDC_MULTI_SESSION", CDC_MULTI_SESSION },
            { "CDC_MCN", CDC_MCN },
            { "CDC_MEDIA_CHANGED", CDC_MEDIA_CHANGED },
            { "CDC_PLAY_AUDIO", CDC_PLAY_AUDIO },
            { "CDC_RESET", CDC_RESET },
            { "CDC_DRIVE_STATUS", CDC_DRIVE_STATUS },
            { "CDC_GENERIC_PACKET", CDC_GENERIC_PACKET },
            { "CDC_CD_R", CDC_CD_R },
            { "CDC_CD_RW", CDC_CD_RW },
            { "CDC_DVD", CDC_DVD },
            { "CDC_DVD_R", CDC_DVD_R },
            { "CDC_DVD_RAM", CDC_DVD_RAM },
            { "CDC_MO_DRIVE", CDC_MO_DRIVE },
            { "CDC_MRW", CDC_MRW },
            { "CDC_MRW_W", CDC_MRW_W },
            { "CDC_RAM", CDC_RAM },
        };

        explain_string_buffer_t buf;
        char            text[1000];

        explain_string_buffer_init(&buf, text, sizeof(text));
        explain_parse_bits_print(&buf, capability, table, SIZEOF(table));
        report("CDROM_GET_CAPABILITY", "Capability", text);
    }
}

#endif

#ifdef DVD_READ_STRUCT

static void
dvd_read_struct(int fildes)
{
    dvd_struct      data;
    int             j;

    data.type = DVD_STRUCT_PHYSICAL;
    for (j = 0; j < DVD_LAYERS; ++j)
    {
        data.physical.layer_num = 0;
        if (ioctl(fildes, DVD_READ_STRUCT, &data) >= 0)
        {
            struct dvd_layer *p;

            p = &data.physical.layer[j];
            report_int("DVD_READ_STRUCT", "Physcial Layer", j, 0);
            report_int("DVD_READ_STRUCT", "    Book version", p->book_version,
                0);
            report_int("DVD_READ_STRUCT", "    Book type", p->book_type, 0);
            report_int("DVD_READ_STRUCT", "    Mininimum rate", p->min_rate, 0);
            report_int("DVD_READ_STRUCT", "    Disc size", p->disc_size, 0);
            report_int("DVD_READ_STRUCT", "    Layer type", p->layer_type, 0);
            report_int("DVD_READ_STRUCT", "    Track path", p->track_path, 0);
            report_int("DVD_READ_STRUCT", "    Number of layers", p->nlayers,
                0);
            report_int("DVD_READ_STRUCT", "    Track density", p->track_density,
                0);
            report_int("DVD_READ_STRUCT", "    Linear density",
                p->linear_density, 0);
            report_int("DVD_READ_STRUCT", "    Start sector", p->start_sector,
                0);
            report_int("DVD_READ_STRUCT", "    End sector", p->end_sector, 0);
            report_int("DVD_READ_STRUCT", "    End sector L0", p->end_sector_l0,
                0);
            report_int("DVD_READ_STRUCT", "    Bca", p->bca, 0);
        }
    }

    data.type = DVD_STRUCT_COPYRIGHT;
    for (j = 0; j < DVD_LAYERS; ++j)
    {
        data.copyright.layer_num = 0;
        if (ioctl(fildes, DVD_READ_STRUCT, &data) >= 0)
        {
            report_int("DVD_READ_STRUCT", "Copyright Layer", j, 0);
            report_int("DVD_READ_STRUCT", "    cpst", data.copyright.cpst, 0);
            report_int("DVD_READ_STRUCT", "    rmi", data.copyright.rmi, 0);
        }
    }

    data.type = DVD_STRUCT_DISCKEY;
    for (j = 0; j < 4; ++j)
    {
        data.disckey.agid = j;
        if (ioctl(fildes, DVD_READ_STRUCT, &data) >= 0)
        {
            char            text[sizeof(data.disckey.value) * 3 + 1];
            explain_string_buffer_t sb;

            report_int("DVD_READ_STRUCT", "Disc Key", j, 0);
            explain_string_buffer_init(&sb, text, sizeof(text));
            explain_buffer_hexdump(&sb, data.disckey.value,
                sizeof(data.disckey.value));
            report("DVD_READ_STRUCT", "    Value", text);
        }
    }

    data.type = DVD_STRUCT_BCA;
    if (ioctl(fildes, DVD_READ_STRUCT, &data) >= 0)
    {
        int             n;
        explain_string_buffer_t sb;
        char            text[sizeof(data.bca.value) * 3 + 2];

        explain_string_buffer_init(&sb, text, sizeof(text));
        n = data.bca.len;
        if (n > (int)sizeof(data.bca.value))
            n = sizeof(data.bca.value);
        if (n > 0)
            explain_buffer_hexdump(&sb, data.bca.value, n);
        report("DVD_READ_STRUCT", "BCA", text);
    }

    data.type = DVD_STRUCT_MANUFACT;
    for (j = 0; j < DVD_LAYERS; ++j)
    {
        data.manufact.layer_num = j;
        if (ioctl(fildes, DVD_READ_STRUCT, &data) >= 0)
        {
            int             n;
            explain_string_buffer_t sb;
            char            text[sizeof(data.manufact.value) * 3 + 1];

            report_int("DVD_READ_STRUCT", "Manufacturer", j, 0);
            explain_string_buffer_init(&sb, text, sizeof(text));
            n = data.manufact.len;
            if (n > (int)sizeof(data.manufact.value))
                n = sizeof(data.manufact.value);
            if (n > 0)
                explain_buffer_hexdump(&sb, data.disckey.value, n);
            report("DVD_READ_STRUCT", "    Value", text);
        }
    }
}

#endif


void
scan_linux_cdrom(int fildes)
{
    (void)fildes;
#ifndef HAVE_LINUX_CDROM_H
    (void)fildes;
#endif
#ifdef CDROMGETSPINDOWN
    report_semi_auto(fildes, CDROMGETSPINDOWN);
#endif
#ifdef CDROMREADTOCHDR
    cdromreadtochdr(fildes);
#endif
#ifdef CDROMREADTOCENTRY
    cdromreadtocentry(fildes);
#endif
#ifdef CDROMVOLREAD
    report_semi_auto(fildes, CDROMVOLREAD);
#endif
#ifdef CDROM_CHANGER_NSLOTS
    cdrom_changer_nslots(fildes);
#endif
#ifdef CDROM_DISC_STATUS
    cdrom_disc_status(fildes);
#endif
#ifdef CDROM_DRIVE_STATUS
    cdrom_drive_status(fildes);
#endif
#ifdef CDROM_GET_CAPABILITY
    cdrom_get_capability(fildes);
#endif
#ifdef CDROM_GET_MCN
    report_semi_auto(fildes, CDROM_GET_MCN);
#endif
#ifdef CDROM_LAST_WRITTEN
    report_semi_auto(fildes, CDROM_LAST_WRITTEN);
#endif
#ifdef CDROM_MEDIA_CHANGED
    report_semi_auto(fildes, CDROM_MEDIA_CHANGED);
#endif
#ifdef CDROM_NEXT_WRITABLE
    report_semi_auto(fildes, CDROM_NEXT_WRITABLE);
#endif
#ifdef DVD_READ_STRUCT
    dvd_read_struct(fildes);
#endif
}
