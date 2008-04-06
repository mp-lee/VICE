/*
 * iecbus.c - Common IEC bus emulation.
 *
 * Written by
 *  Andreas Boose <viceteam@t-online.de>
 *
 * This file is part of VICE, the Versatile Commodore Emulator.
 * See README for copyright notice.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *  02111-1307  USA.
 *
 */

#include "vice.h"

#include "fsdrive.h"
#include "iecbus.h"
#include "realdevice.h"
#include "serial.h"
#include "types.h"


void iecbus_open(unsigned int device, BYTE secondary, void(*st_func)(BYTE))
{
#if HAVE_OPENCBM
    if (serial_device_type_get(device & 0x0f) == SERIAL_DEVICE_REAL)
        realdevice_open(device, secondary, st_func);
    else
#endif
        fsdrive_open(device, secondary, st_func);
}

void iecbus_close(unsigned int device, BYTE secondary, void(*st_func)(BYTE))
{
#if HAVE_OPENCBM
    if (serial_device_type_get(device & 0x0f) == SERIAL_DEVICE_REAL)
        realdevice_close(device, secondary, st_func);
    else
#endif
        fsdrive_close(device, secondary, st_func);
}

void iecbus_listentalk(unsigned int device, BYTE secondary,
                       void(*st_func)(BYTE))
{
#if HAVE_OPENCBM
    if (serial_device_type_get(device & 0x0f) == SERIAL_DEVICE_REAL)
        realdevice_listentalk(device, secondary, st_func);
    else
#endif
        fsdrive_listentalk(device, secondary, st_func);
}

void iecbus_unlisten(unsigned int device, BYTE secondary, void(*st_func)(BYTE))
{
#if HAVE_OPENCBM
    if (serial_device_type_get(device & 0x0f) == SERIAL_DEVICE_REAL)
        realdevice_unlisten(st_func);
    else
#endif
        fsdrive_unlisten(device, secondary, st_func);
}

void iecbus_untalk(unsigned int device, BYTE secondary, void(*st_func)(BYTE))
{
#if HAVE_OPENCBM
    if (serial_device_type_get(device & 0x0f) == SERIAL_DEVICE_REAL)
        realdevice_untalk(st_func);
    else
#endif
        fsdrive_untalk(device, secondary, st_func);
}

void iecbus_write(unsigned int device, BYTE secondary, BYTE data,
                  void(*st_func)(BYTE))
{
#if HAVE_OPENCBM
    if (serial_device_type_get(device & 0x0f) == SERIAL_DEVICE_REAL)
        realdevice_write(data, st_func);
    else
#endif
        fsdrive_write(device, secondary, data, st_func);
}

BYTE iecbus_read(unsigned int device, BYTE secondary, void(*st_func)(BYTE))
{
#if HAVE_OPENCBM
    if (serial_device_type_get(device & 0x0f) == SERIAL_DEVICE_REAL)
        return realdevice_read(st_func);
    else
#endif
        return fsdrive_read(device, secondary, st_func);
}

void iecbus_reset(void)
{
    fsdrive_reset();
#if HAVE_OPENCBM
    realdevice_reset();
#endif
}

void iecbus_init(void)
{
    fsdrive_init();
#if HAVE_OPENCBM
    realdevice_init();
#endif
}

