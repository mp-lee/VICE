/*
 * uicmdline.c - The command line user interface.
 *
 * Written by
 *  Andreas Boose <viceteam@t-online.de>
 *  Thomas Bretz <tbretz@gsi.de>
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

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "cmdline.h"
#include "intl.h"
#include "lib.h"
#include "translate.h"
#include "uilib.h"


/* This does not work as stdout is directed to null.
   This function is temporarily extended during the
   transition to string tables.  */

void ui_cmdline_show_help(unsigned int num_options,
                          cmdline_option_ram_t *options,
                          void *userparam)
{
    unsigned int i;
    int has_param;
    char *description;

    printf("\nAvailable command-line options:\n\n");
    for (i = 0; i < num_options; i++) {
        fputs(options[i].name, stdout);
        description = (options[i].use_description_id == 1) ? translate_text(options[i].description_trans) : (char *)options[i].description;

        if (options[i].use_param_name_id == 1)
        {
            if (options[i].param_name_trans != 0)
                has_param = 1;
            else
                has_param = 0;
        }
        else
        {
            if (options[i].param_name != NULL)
                has_param = 1;
            else
                has_param = 0;
        }

        if (options[i].need_arg && has_param != 0)
            printf(" %s", (options[i].use_param_name_id == 1) ? translate_text(options[i].param_name_trans) : options[i].param_name);
        printf("\n\t%s\n", description);
    }
    putchar('\n');
}
