/*
 * c64ui.c - Implementation of the C64-specific part of the UI.
 *
 * Written by
 *  Ettore Perazzoli <ettore@comm2000.it>
 *  Andr� Fachat <fachat@physik.tu-chemnitz.de>
 *  Andreas Boose <boose@linux.rz.fh-hannover.de>
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

#include <stdio.h>
#include <stdlib.h>

#include "c64mem.h"
#include "drive.h"
#include "icon.h"
#include "joystick.h"
#include "resources.h"
#include "uic64cart.h"
#include "uicommands.h"
#include "uidatasette.h"
#include "uimenu.h"
#include "uireu.h"
#include "uiscreenshot.h"
#include "uisettings.h"
#include "uisid.h"
#include "uivicii.h"
#include "utils.h"
#include "vsync.h"


/* ------------------------------------------------------------------------- */

UI_MENU_DEFINE_RADIO(VideoStandard)

ui_menu_entry_t set_video_standard_submenu[] = {
    { N_("*PAL-G"), (ui_callback_t)radio_VideoStandard,
      (ui_callback_data_t)DRIVE_SYNC_PAL, NULL },
    { N_("*NTSC-M"), (ui_callback_t)radio_VideoStandard,
      (ui_callback_data_t)DRIVE_SYNC_NTSC, NULL },
    { N_("*Old NTSC-M"), (ui_callback_t)radio_VideoStandard,
      (ui_callback_data_t)DRIVE_SYNC_NTSCOLD, NULL },
    { NULL }
};

/* ------------------------------------------------------------------------- */

UI_MENU_DEFINE_TOGGLE(SidStereo)

UI_MENU_DEFINE_TOGGLE(SidFilters)

#ifdef HAVE_RESID
UI_MENU_DEFINE_TOGGLE(SidUseResid)
#endif

ui_menu_entry_t sid_submenu[] = {
    { N_("*Second SID at $DE00"),
      (ui_callback_t)toggle_SidStereo, NULL, NULL },
    { "--" },
    { N_("*Emulate filters"),
      (ui_callback_t)toggle_SidFilters, NULL, NULL },
    { N_("Chip model"),
      NULL, NULL, sid_model_submenu },
#ifdef HAVE_RESID
    { "--" },
    { N_("*Use reSID emulation"),
      (ui_callback_t)toggle_SidUseResid, NULL, NULL },
    { N_("reSID sampling method"),
      NULL, NULL, sid_resid_sampling_submenu },
    { N_("reSID resampling passband..."),
      (ui_callback_t)set_sid_resid_passband, NULL, NULL },
#endif
    { NULL },
};

UI_MENU_DEFINE_TOGGLE(Sound)

static ui_menu_entry_t sid_options_submenu[] = {
    { N_("*Enable sound playback"),
      (ui_callback_t)toggle_Sound, NULL, NULL },
    { N_("*Second SID at $DE00"),
      (ui_callback_t)toggle_SidStereo, NULL, NULL },
#ifdef HAVE_RESID
    { N_("*Use reSID emulation"),
      (ui_callback_t)toggle_SidUseResid, NULL, NULL },
#endif
    { N_("*Emulate filters"),
      (ui_callback_t)toggle_SidFilters, NULL, NULL },
    { N_("Chip model"),
      NULL, NULL, sid_model_submenu },
    { NULL }
};

/* ------------------------------------------------------------------------- */

UI_MENU_DEFINE_TOGGLE(EmuID)
#ifdef HAVE_MOUSE
UI_MENU_DEFINE_TOGGLE(Mouse)
#endif

static ui_menu_entry_t io_extensions_submenu[] = {
    { N_("RAM Expansion Unit"),
      NULL, NULL, reu_submenu },
    { N_("*Emulator identification"),
      (ui_callback_t)toggle_EmuID, NULL, NULL },
#ifdef HAVE_MOUSE
    { N_("*1351 Mouse Emulation"),
      (ui_callback_t)toggle_Mouse, NULL, NULL, XK_m, UI_HOTMOD_META },
#endif
    { NULL }
};

/* ------------------------------------------------------------------------- */


static UI_CALLBACK(set_joystick_device_1)
{
    int tmp;

    vsync_suspend_speed_eval();
    if (!CHECK_MENUS) {
        resources_set_value("JoyDevice1", (resource_value_t)UI_MENU_CB_PARAM);
        ui_update_menus();
    } else {
        resources_get_value("JoyDevice1", (resource_value_t *)&tmp);
        ui_menu_set_tick(w, tmp == (int)UI_MENU_CB_PARAM);
    }
}

static UI_CALLBACK(set_joystick_device_2)
{
    int tmp;

    vsync_suspend_speed_eval();
    if (!CHECK_MENUS) {
        resources_set_value("JoyDevice2", (resource_value_t)UI_MENU_CB_PARAM);
        ui_update_menus();
    } else {
        resources_get_value("JoyDevice2", (resource_value_t *)&tmp);
        ui_menu_set_tick(w, tmp == (int)UI_MENU_CB_PARAM);
    }
}

static UI_CALLBACK(swap_joystick_ports)
{
    int tmp1, tmp2;

    if (w != NULL)
        vsync_suspend_speed_eval();
    resources_get_value("JoyDevice1", (resource_value_t *)&tmp1);
    resources_get_value("JoyDevice2", (resource_value_t *)&tmp2);
    resources_set_value("JoyDevice1", (resource_value_t)tmp2);
    resources_set_value("JoyDevice2", (resource_value_t)tmp1);
    ui_update_menus();
}

static ui_menu_entry_t set_joystick_device_1_submenu[] = {
    { N_("*None"),
      (ui_callback_t)set_joystick_device_1,
      (ui_callback_data_t)JOYDEV_NONE, NULL },
    { N_("*Numpad"),
      (ui_callback_t)set_joystick_device_1,
      (ui_callback_data_t)JOYDEV_NUMPAD, NULL },
    { N_("*Custom Keys"),
      (ui_callback_t)set_joystick_device_1,
      (ui_callback_data_t)JOYDEV_CUSTOM_KEYS, NULL },
#ifdef HAS_JOYSTICK
    { N_("*Analog Joystick 0"),
      (ui_callback_t)set_joystick_device_1,
      (ui_callback_data_t)JOYDEV_ANALOG_0, NULL },
    { N_("*Analog Joystick 1"),
      (ui_callback_t)set_joystick_device_1,
      (ui_callback_data_t)JOYDEV_ANALOG_1, NULL },
#ifdef HAS_DIGITAL_JOYSTICK
    { N_("*Digital Joystick 0"),
      (ui_callback_t)set_joystick_device_1,
      (ui_callback_data_t)JOYDEV_DIGITAL_0, NULL },
    { N_("*Digital Joystick 1"),
      (ui_callback_t)set_joystick_device_1,
      (ui_callback_data_t)JOYDEV_DIGITAL_1, NULL },
#endif
#endif
    { NULL }
};

static ui_menu_entry_t set_joystick_device_2_submenu[] = {
    { N_("*None"),
      (ui_callback_t)set_joystick_device_2,
      (ui_callback_data_t)JOYDEV_NONE, NULL },
    { N_("*Numpad"),
      (ui_callback_t)set_joystick_device_2,
      (ui_callback_data_t)JOYDEV_NUMPAD, NULL },
    { N_("*Custom Keys"),
      (ui_callback_t)set_joystick_device_2,
      (ui_callback_data_t)JOYDEV_CUSTOM_KEYS, NULL },
#ifdef HAS_JOYSTICK
    { N_("*Analog Joystick 0"),
      (ui_callback_t)set_joystick_device_2,
      (ui_callback_data_t)JOYDEV_ANALOG_0, NULL },
    { N_("*Analog Joystick 1"),
      (ui_callback_t)set_joystick_device_2,
      (ui_callback_data_t)JOYDEV_ANALOG_1, NULL },
#ifdef HAS_DIGITAL_JOYSTICK
    { N_("*Digital Joystick 0"),
      (ui_callback_t)set_joystick_device_2,
      (ui_callback_data_t)JOYDEV_DIGITAL_0, NULL },
    { N_("*Digital Joystick 1"),
      (ui_callback_t)set_joystick_device_2,
      (ui_callback_data_t)JOYDEV_DIGITAL_1, NULL },
#endif
#endif /* HAS_JOYSTICK */
    { NULL }
};


static ui_menu_entry_t joystick_settings_submenu[] = {
    { N_("Joystick device in port 1"),
      NULL, NULL, set_joystick_device_1_submenu },
    { N_("Joystick device in port 2"),
      NULL, NULL, set_joystick_device_2_submenu },
    { "--" },
    { N_("Swap joystick ports"),
      (ui_callback_t)swap_joystick_ports, NULL, NULL, XK_j, UI_HOTMOD_META },
    { NULL }
};

static ui_menu_entry_t joystick_options_submenu[] = {
    { N_("Swap joystick ports"),
      (ui_callback_t)swap_joystick_ports, NULL, NULL, XK_j, UI_HOTMOD_META },
    { NULL }
};

static ui_menu_entry_t joystick_settings_menu[] = {
    { N_("Joystick settings"),
      NULL, NULL, joystick_settings_submenu },
    { NULL }
};

/* ------------------------------------------------------------------------- */

static ui_menu_entry_t c64_romset_submenu[] = {
    { N_("Load default ROMs"),
      (ui_callback_t)ui_set_romset,
      (ui_callback_data_t)"default.vrs", NULL },
    { "--" },
    { N_("Load new kernal ROM"),
      (ui_callback_t)ui_load_rom_file,
      (ui_callback_data_t)"KernalName", NULL },
    { N_("Load new BASIC ROM"),
      (ui_callback_t)ui_load_rom_file,
      (ui_callback_data_t)"BasicName", NULL },
    { N_("Load new character ROM"),
      (ui_callback_t)ui_load_rom_file,
      (ui_callback_data_t)"ChargenName", NULL },
    { "--" },
    { N_("Load new 1541 ROM"),
      (ui_callback_t)ui_load_rom_file,
      (ui_callback_data_t)"DosName1541", NULL },
    { N_("Load new 1541-II ROM"),
      (ui_callback_t)ui_load_rom_file,
      (ui_callback_data_t)"DosName1541ii", NULL },
    { N_("Load new 1571 ROM"),
      (ui_callback_t)ui_load_rom_file,
      (ui_callback_data_t)"DosName1571", NULL },
    { N_("Load new 1581 ROM"),
      (ui_callback_t)ui_load_rom_file,
      (ui_callback_data_t)"DosName1581", NULL },
    { N_("Load new 2031 ROM"),
      (ui_callback_t)ui_load_rom_file,
      (ui_callback_data_t)"DosName2031", NULL },
    { "--" },
    { N_("Load custom ROM set from file"),
      (ui_callback_t)ui_load_romset, NULL, NULL },
    { N_("Dump ROM set definition to file"),
      (ui_callback_t)ui_dump_romset, NULL, NULL },
    { NULL }
};

/* ------------------------------------------------------------------------- */

static UI_CALLBACK(save_screenshot)
{
    /* Where does the 1024 come from?  */
    char filename[1024];
    int wid = (int)UI_MENU_CB_PARAM;

    vsync_suspend_speed_eval();

    /* The following code depends on a zeroed filename.  */
    memset(filename, 0, 1024);

    if (ui_screenshot_dialog(filename, wid) < 0)
        return;
}

static ui_menu_entry_t ui_screenshot_commands_menu[] = {
    { N_("Screenshot..."),
      (ui_callback_t)save_screenshot, (ui_callback_data_t)0, NULL },
    { NULL }
};

/* ------------------------------------------------------------------------- */

static ui_menu_entry_t c64_menu[] = {
    { N_("ROM settings"),
      NULL, NULL, c64_romset_submenu },
    { N_("VIC-II settings"),
      NULL, NULL, vic_submenu },
    { N_("SID settings"),
      NULL, NULL, sid_submenu },
    { N_("I/O extensions at $DFxx"),
      NULL, NULL, io_extensions_submenu },
    { N_("RS232 settings"),
      NULL, NULL, rs232_submenu },
    { NULL }
};

static ui_menu_entry_t c64_settings_menu[] = {
    { N_("ROM settings"),
      NULL, NULL, c64_romset_submenu },
    { N_("VIC-II settings"),
      NULL, NULL, vic_submenu },
    { N_("SID settings"),
      NULL, NULL, sid_submenu },
    { N_("RS232 settings"),
      NULL, NULL, rs232_submenu },
    { NULL }
};

int c64_ui_init(void)
{
    ui_set_application_icon(c64_icon_data);
    ui_set_left_menu(ui_menu_create("LeftMenu",
                                    ui_disk_commands_menu,
                                    ui_menu_separator,
                                    ui_tape_commands_menu,
                                    ui_datasette_commands_menu,
                                    ui_menu_separator,
                                    ui_smart_attach_commands_menu,
                                    ui_menu_separator,
                                    ui_c64cart_commands_menu,
                                    ui_menu_separator,
                                    ui_directory_commands_menu,
                                    ui_menu_separator,
                                    ui_snapshot_commands_menu,
                                    ui_screenshot_commands_menu,
                                    ui_menu_separator,
                                    ui_tool_commands_menu,
                                    ui_menu_separator,
                                    ui_help_commands_menu,
                                    ui_menu_separator,
                                    ui_run_commands_menu,
                                    ui_menu_separator,
                                    ui_exit_commands_menu,
                                    NULL));

    ui_set_right_menu(ui_menu_create("RightMenu",
                                     ui_performance_settings_menu,
                                     ui_menu_separator,
                                     ui_video_settings_menu,
#ifdef USE_XF86_EXTENSIONS
                                     ui_fullscreen_settings_menu,
#endif
                                     ui_keyboard_settings_menu,
                                     ui_sound_settings_menu,
                                     ui_drive_settings_menu,
                                     ui_peripheral_settings_menu,
                                     joystick_settings_menu,
                                     ui_menu_separator,
                                     c64_menu,
                                     ui_menu_separator,
                                     ui_settings_settings_menu,
                                     NULL));

    ui_set_topmenu("TopLevelMenu",
                   _("File"),
                   ui_menu_create("File",
                                  ui_smart_attach_commands_menu,
                                  ui_menu_separator,
                                  ui_disk_commands_menu,
                                  ui_menu_separator,
                                  ui_tape_commands_menu,
                                  ui_datasette_commands_menu,
                                  ui_menu_separator,
                                  ui_c64cart_commands_menu,
                                  ui_menu_separator,
                                  ui_directory_commands_menu,
                                  ui_menu_separator,
                                  ui_tool_commands_menu,
                                  ui_menu_separator,
                                  ui_run_commands_menu,
                                  ui_menu_separator,
                                  ui_exit_commands_menu,
                                  NULL),
                   _("Snapshot"),
                   ui_menu_create("Snapshot",
                                  ui_snapshot_commands_submenu,
                                  ui_menu_separator,
                                  ui_screenshot_commands_menu,
                                  NULL),
                   _("Options"),
                   ui_menu_create("Options",
                                  ui_performance_settings_menu,
                                  ui_menu_separator,
#ifdef USE_XF86_EXTENSIONS
                                  ui_fullscreen_settings_menu,
                                  ui_menu_separator,
#endif
                                  joystick_options_submenu,
                                  ui_menu_separator,
                                  sid_options_submenu,
                                  ui_menu_separator,
                                  ui_drive_options_submenu,
                                  ui_menu_separator,
                                  io_extensions_submenu,
                                  NULL),
                   _("Settings"),
                   ui_menu_create("Settings",
                                  ui_peripheral_settings_menu,
                                  ui_drive_settings_menu,
                                  ui_keyboard_settings_menu,
                                  joystick_settings_menu,
                                  ui_sound_settings_menu,
                                  ui_menu_separator,
                                  c64_settings_menu,
                                  ui_menu_separator,
                                  ui_settings_settings_menu,
                                  NULL),
                   /* Translators: RJ means right justify and should be
                      saved in your tranlation! e.g. german "RJHilfe" */
                   _("RJHelp"),
                   ui_menu_create("Help",
                                  ui_help_commands_menu,
                                  NULL),
                   NULL);

    ui_set_speedmenu(ui_menu_create("SpeedMenu",
                                    ui_performance_settings_menu,
                                    ui_menu_separator,
                                    video_settings_submenu,
                                    ui_menu_separator,
                                    NULL));
    ui_set_tape_menu(ui_menu_create("TapeMenu",
                                    ui_tape_commands_menu,
                                    ui_menu_separator,
                                    datasette_control_submenu,
                                    NULL));

    ui_update_menus();

    return 0;
}

