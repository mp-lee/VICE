/*
 * rs232drv.c - Common RS232 driver handling.
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

#ifndef _RS232DRV_H
#define _RS232DRV_H

extern int rs232drv_resources_init(void);
extern int rs232drv_cmdline_options_init(void);

extern void rs232drv_init(void);
extern void rs232drv_reset(void);
extern int rs232drv_open(int device);
extern void rs232drv_close(int fd);
extern int rs232drv_putc(int fd, BYTE b);
extern int rs232drv_getc(int fd, BYTE *b);

#endif

