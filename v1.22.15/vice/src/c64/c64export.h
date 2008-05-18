/*
 * c64export.h - Expansion port handling for the C64.
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

#ifndef _C64EXPORT_H
#define _C64EXPORT_H

struct c64export_resource_s {
    const char *name;
    unsigned int use_roml;
    unsigned int use_romh;
};
typedef struct c64export_resource_s c64export_resource_t;


extern int c64export_query(const c64export_resource_t *export_res);
extern int c64export_add(const c64export_resource_t *export_res);
extern int c64export_remove(const c64export_resource_t *export_res);
extern int c64export_resources_init(void);

#endif

