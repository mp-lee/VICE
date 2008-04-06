/*
 * vicii-cmdline-options.c - Command-line options for the MOS 6569 (VIC-II)
 * emulation.
 *
 * Written by
 *  Ettore Perazzoli <ettore@comm2000.it>
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
 * */

#include "vice.h"

#include <stdio.h>

#include "archdep.h"
#include "cmdline.h"
#include "raster-cmdline-options.h"
#include "vicii-cmdline-options.h"


/* VIC-II command-line options.  */
static cmdline_option_t cmdline_options[] =
{
    { "-checksb", SET_RESOURCE, 0, NULL, NULL, "VICIICheckSbColl",
      (void *)1, NULL, "Enable sprite-background collision registers" },
    { "+checksb", SET_RESOURCE, 0, NULL, NULL, "VICIICheckSbColl",
      (void *)0, NULL, "Disable sprite-background collision registers" },
    { "-checkss", SET_RESOURCE, 0, NULL, NULL, "VICIICheckSsColl",
      (void *)1, NULL, "Enable sprite-sprite collision registers" },
    { "+checkss", SET_RESOURCE, 0, NULL, NULL, "VICIICheckSsColl",
      (void *)0, NULL, "Disable sprite-sprite collision registers" },
    { "-newluminance", SET_RESOURCE, 0, NULL, NULL, "VICIINewLuminances",
      (void *)1, NULL, "Use new luminances" },
    { "+newluminance", SET_RESOURCE, 0, NULL, NULL, "VICIINewLuminances",
      (void *)0, NULL, "Use old luminances" },
    { "-saturation", SET_RESOURCE, 1, NULL, NULL, "ColorSaturation", NULL,
      "<0-2000>", "Set saturation of internal calculated palette [1000]" },
    { "-contrast", SET_RESOURCE, 1, NULL, NULL, "ColorContrast", NULL,
      "<0-2000>", "Set contrast of internal calculated palette [1100]" },
    { "-brightness", SET_RESOURCE, 1, NULL, NULL, "ColorBrightness", NULL,
      "<0-2000>", "Set brightness of internal calculated palette [1100]" },
    { "-gamma", SET_RESOURCE, 1, NULL, NULL, "ColorGamma", NULL,
      "<0-2000>", "Set gamma of internal calculated palette [900]" },
    { "-intpal", SET_RESOURCE, 0, NULL, NULL, "ExternalPalette",
      (void *)0, NULL, "Use an internal calculated palette" },
    { "-extpal", SET_RESOURCE, 0, NULL, NULL, "ExternalPalette",
      (void *)1, NULL, "Use an external palette (file)" },
    { "-palette", SET_RESOURCE, 1, NULL, NULL, "PaletteFile",
      NULL, "<name>", "Specify name of file of external palette" },
    { NULL }
};


/* VIC-II double-size-specific command-line options.  */
static cmdline_option_t cmdline_options_2x[] =
{
#if ARCHDEP_VICII_DSIZE == 1
    { "-dsize", SET_RESOURCE, 0, NULL, NULL, "DoubleSize",
      (void *)1, NULL, "Enable double size" },
    { "+dsize", SET_RESOURCE, 0, NULL, NULL, "DoubleSize",
      (void *)0, NULL, "Disable double size" },
#endif
#if ARCHDEP_VICII_DSCAN == 1
    { "-dscan", SET_RESOURCE, 0, NULL, NULL, "VICIIDoubleScan",
      (void *)1, NULL, "Enable double scan" },
    { "+dscan", SET_RESOURCE, 0, NULL, NULL, "VICIIDoubleScan",
      (void *)0, NULL, "Disable double scan" },
#endif
    { NULL }
};

int vic_ii_cmdline_options_init(void)
{
#if (ARCHDEP_VICII_DSIZE == 1) || (ARCHDEP_VICII_DSCAN == 1)
    if (cmdline_register_options(cmdline_options_2x) < 0)
        return -1;
#endif
    if (raster_cmdline_options_chip_init("VICII") < 0)
        return -1;

    return cmdline_register_options(cmdline_options);
}

