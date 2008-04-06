/*
 * menubar.c - The menubar.
 *
 * Written by
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

#define INCL_WINHELP       // WinQueryHelpInstance
#define INCL_WINMENUS      // WinCheckMenuItem
#define INCL_WINDIALOGS    // WinMessageBox
#define INCL_WINWINDOWMGR  // QWL_USER
#include "videoarch.h"     // canvas_*
//#include "video.h"         // canvas_*
#include "dialogs.h"
#include "menubar.h"
#include "dlg-color.h"
#include "dlg-drive.h"
#include "dlg-monitor.h"
//#include "dlg-emulator.h"
#include "dlg-joystick.h"
#include "dlg-datasette.h"

#include "snippets\\pmwin2.h"

#include <string.h>      // strcmp

#include "log.h"

#include "mon.h"         // mon
#include "tape.h"        // tape_detach_image
#include "drive.h"       // DRIVE_SYNC_*
#include "utils.h"       // xmsprintf
#include "sound.h"       // SOUND_ADJUST_*
#include "attach.h"      // file_system_detach_disk
#include "archdep.h"     // archdep_boot_path
#include "machine.h"     // machine_read/write_snapshot
#include "cmdline.h"     // cmdline_show_help, include resources.h
#include "vsyncapi.h"    // vsyncarch
#include "fliplist.h"    // flip_attach_head
#include "cartridge.h"   // CARTRIDGE_*
#include "interrupt.h"   // maincpu_trigger_trap
#include "screenshot.h"  // screenshot_canvas_save
#include "dlg-fileio.h"  // ViceFileDialog

// --------------------------------------------------------------------------

#ifdef __XCBM__
#include "c610mem.h"     // cbm2_set_model
const char cbm_models[][5] = { "510", "610", "620", "620+", "710", "720", "720+" };
static void set_cbm_model(ADDRESS addr, void *model)
{
    cbm2_set_model((char*)model, NULL);
}
#endif


#ifdef __XPET__
#include "pets.h"
const char pet_models[][10] = { "2001", "3008", "3016", "3032", "3032B", "4016",
                                "4032", "4032B", "8032", "8096", "8296", "SuperPET" };
static void set_pet_model(ADDRESS addr, void *model)
{
    pet_set_model((char*)model, NULL);
}
#endif

#if defined __X64__ || defined __X128__ || defined __XVIC__
static const char *VIDEO_CACHE="VideoCache";
#else // __XPET__ || __XCBM__
static const char *VIDEO_CACHE="CrtcVideoCache";
#endif

extern void set_volume(int vol);
extern int  get_volume(void);

// --------------------------------------------------------------------------

static void mon_trap(ADDRESS addr, void *unused_data)
{
    mon(addr);
}

static void load_snapshot(ADDRESS addr, void *hwnd)
{
    char *name = concat(archdep_boot_path(), "\\vice2.vsf", NULL);
    if (machine_read_snapshot(name) < 0)
        WinMessageBox(HWND_DESKTOP, (HWND)hwnd,
                      "Unable to load snapshot - sorry!",
                      "Load Snapshot", 0, MB_OK);
    else
        log_debug("Snapshot '%s' loaded successfully.", name);
    free(name);
}

static void save_snapshot(ADDRESS addr, void *hwnd)
{
    // FIXME !!!!! roms, disks
    char *name = concat(archdep_boot_path(), "\\vice2.vsf", NULL);
    if (machine_write_snapshot(name, 1, 1) < 0)
            WinMessageBox(HWND_DESKTOP, (HWND)hwnd,
                          "Unable to save snapshot - sorry!",
                          "Save Snapshot", 0, MB_OK);
    else
        log_debug("Snapshot saved as '%s' successfully.", name);
    free(name);
}

void save_screenshot(HWND hwnd)
{
    char *name = concat(archdep_boot_path(), "\\vice2.png", NULL);
    if (!screenshot_canvas_save("PNG", name, (canvas_t *)WinQueryWindowPtr(hwnd, QWL_USER)))
        log_debug("Screenshot saved as '%s' successfully.", name);
    free(name);
}

// --------------------------------------------------------------------------

char *printer_res(const char *res, const int num)
{
    switch(num)
    {
    case 0:
        return xmsprintf(res, "4");
    case 1:
        return xmsprintf(res, "5");
    case 2:
        return xmsprintf(res, "Userport");
    }
    log_debug("Res: %d", num);
    return xmsprintf(res, "ERROR");
}

void set_printer_res(const char *res, int num, resource_value_t *val)
{
    char *txt = printer_res(res, num);
    resources_set_value(txt, val);
    free(txt);
}

void get_printer_res(const char *res, int num, resource_value_t *val)
{
    char *txt = printer_res(res, num);
    resources_get_value(txt, val);
    free(txt);
}

// --------------------------------------------------------------------------

void ChangeSpeed(HWND hwnd, int idm)
{
    static ULONG tm    = 0; //vsyncarch_gettime();
    static int   step  = 1;
    static int   calls = 0;

    const canvas_t *c = (canvas_t *)WinQueryWindowPtr(hwnd, QWL_USER);

    ULONG time = vsyncarch_gettime();

    char *txt;

    int speed;
    resources_get_value("Speed", (resource_value_t*) &speed);

    if ((signed long)(time-tm) < vsyncarch_frequency())
    {
        if (calls==25)
        {
            calls=0;
            step*=5;

            if (step>100)
                step=100;
        }
    }
    else
    {
        step=1;
        calls=0;
    }
    tm=time;

    calls++;

    speed += idm==IDM_PLUS ? step : -step;

    if (speed<0)
        speed=0;

    resources_set_value("Speed", (resource_value_t)speed);

    if (speed)
        txt=xmsprintf("%s - Set Max.Speed: %d%%", c->title, speed);
    else
        txt=xmsprintf("%s - Set Max.Speed: unlimited", c->title);
    WinSetWindowText(c->hwndTitlebar, txt);
    free(txt);
}

// --------------------------------------------------------------------------

void menu_action(HWND hwnd, USHORT idm) //, MPARAM mp2)
{
    switch (idm)
    {
    case IDM_PLUS:
    case IDM_MINUS:
        ChangeSpeed(hwnd, idm);
        return;

    case IDM_FILEOPEN:
        ViceFileDialog(hwnd, 0, FDS_OPEN_DIALOG);
        return;

    case IDM_FILESAVE:
        ViceFileDialog(hwnd, 0, FDS_ENABLEFILELB | FDS_SAVEAS_DIALOG);
        return;

    case IDM_ATTACHTAPE:
        ViceFileDialog(hwnd, 0x0201, FDS_OPEN_DIALOG);
        return;

    case IDM_ATTACH8:
    case IDM_ATTACH9:
    case IDM_ATTACH10:
    case IDM_ATTACH11:
        ViceFileDialog(hwnd, 0x0100|(idm&0xf), FDS_OPEN_DIALOG);
        return;

    case IDM_DETACHTAPE:
        tape_detach_image();
        return;

    case IDM_DETACH8:
    case IDM_DETACH9:
    case IDM_DETACH10:
    case IDM_DETACH11:
        file_system_detach_disk(idm&0xf);
        return;

    case IDM_DETACHALL:
        file_system_detach_disk(8);
        file_system_detach_disk(9);
        file_system_detach_disk(10);
        file_system_detach_disk(11);
        return;

    case IDM_LOGWIN:
        WinActivateWindow(hwndLog, 1);
        return;

#ifdef __X64__
    case IDM_CRTGEN:
    case IDM_CRTGEN8KB:
    case IDM_CRTGEN16KB:
    case IDM_CRTACTREPL:
    case IDM_CRTATOMPOW:
    case IDM_CRTEPYX:
    case IDM_CRTSSSHOT:
    case IDM_CRTSSSHOT5:
    case IDM_CRTWEST:
    case IDM_CRTIEEE:
        ViceFileDialog(hwnd, 0x0b00 /*| (idm&0xf)*/, FDS_OPEN_DIALOG);
        return;
    case IDM_CRTEXPERT:
        {
            int val;
            resources_get_value("CartridgeType", (resource_value_t*) &val);
            if (val!=CARTRIDGE_EXPERT)
            {
                cartridge_attach_image(CARTRIDGE_EXPERT, NULL);
                return;
            }
        }
        // FALLTHROUGH!
    case IDM_CARTRIDGEDET:
        cartridge_detach_image();
        return;
    case IDM_CRTFREEZE:
        cartridge_trigger_freeze();
        return;
    case IDM_CRTSAVEIMG:
        ViceFileDialog(hwnd, 0x0601, FDS_ENABLEFILELB | FDS_SAVEAS_DIALOG);
        return;
#endif

    case IDM_FLIPNEXT8:
    case IDM_FLIPNEXT9:
    case IDM_FLIPPREV8:
    case IDM_FLIPPREV9:
        flip_attach_head((idm>>4)&0xf, idm&1);
        return;

    case IDM_DEFAULTCONFIG:
        resources_set_defaults();
        return;

    case IDM_READCONFIG:
        if (resources_load(NULL)<0)
            WinMessageBox(HWND_DESKTOP, hwnd,
                          "Cannot load default settings.",
                          "Resources", 0, MB_OK);
        return;

    case IDM_WRITECONFIG:
        WinMessageBox(HWND_DESKTOP, hwnd,
                      resources_save(NULL)<0?"Cannot save default settings.":
                      "Settings written successfully.",
                      "Resources", 0, MB_OK);
        return;

    case IDM_SNAPLOAD:
        maincpu_trigger_trap(load_snapshot, (void*)hwnd);
        return;

    case IDM_SNAPSAVE:
        maincpu_trigger_trap(save_snapshot, (void*)hwnd);
        return;

    case IDM_PRINTSCRN:
        save_screenshot(hwnd);
        return;

    case IDM_HARDRESET:
        hardreset_dialog(hwnd);
        return;

    case IDM_SOFTRESET:
        softreset_dialog(hwnd);
        return;

#ifdef __X128__
    case IDM_VDC16K:
    case IDM_VDC64K:
        resources_set_value("VDC_64KB", (resource_value_t*)(idm&1));
        return;

    case IDM_INTFUNCROM:
        toggle("InternalFunctionROM");
        return;
    case IDM_EXTFUNCROM:
        toggle("ExternalFunctionROM");
        return;
#endif

#if defined VIC_II_NEED_2X || defined VIC_NEED_2X
    case IDM_VICDSIZE:
        toggle("DoubleSize");
        return;
    case IDM_VICDSCAN:
        toggle("DoubleScan");
        return;
#endif
#ifdef CRTC_NEED_2X
    case IDM_CRTCDSIZE:
        toggle("CrtcDoubleSize");
        return;
    case IDM_CRTCDSCAN:
        toggle("CrtcDoubleScan");
        return;
#endif

#if defined __X64__ || defined __X128__
        /*
    case IDM_KERNALREV0:
        resources_set_value("KernalRev", "0");
        return;
    case IDM_KERNALREV3:
        resources_set_value("KernalRev", "3");
        return;
    case IDM_KERNALREVSX:
        resources_set_value("KernalRev", "SX");   // "67"
        return;
    case IDM_KERNALREV4064:
        resources_set_value("KernalRev", "4064"); // "100"
        return;
          */
    case IDM_SBCOLL:
        toggle("CheckSbColl");
        return;
    case IDM_SSCOLL:
        toggle("CheckSsColl");
        return;

    case IDM_PAL:
        resources_set_value("VideoStandard",
                            (resource_value_t*) DRIVE_SYNC_PAL);
        return;
    case IDM_NTSC:
        resources_set_value("VideoStandard",
                            (resource_value_t*) DRIVE_SYNC_NTSC);
        return;
#ifdef __X64__
    case IDM_NTSCOLD:
        resources_set_value("VideoStandard",
                            (resource_value_t*) DRIVE_SYNC_NTSCOLD);
        return;
#endif // __X64__

    case IDM_REU:
        toggle("REU");
        return;

    case IDM_REU128:
    case IDM_REU256:
    case IDM_REU512:
    case IDM_REU1024:
    case IDM_REU2048:
    case IDM_REU4096:
    case IDM_REU8192:
    case IDM_REU16384:
        resources_set_value("REUSize",
                            (resource_value_t*)((idm&0xff)<<7));
        return;
#endif // __X64__ || __X128__

    case IDM_MOUSE:
        toggle("Mouse");
        return;

    case IDM_HIDEMOUSE:
        toggle("HideMousePtr");
        return;

    case IDM_PRT4IEC:
    case IDM_PRT5IEC:
    case IDM_PRTUP:
        {
            char *res = printer_res("Printer%s", (idm>>4)&0xf);
            toggle(res);
            free(res);
        }
        return;

    case IDM_PRT4ASCII:
    case IDM_PRT5ASCII:
    case IDM_PRTUPASCII:
        set_printer_res("Printer%sDriver", (idm>>4)&0xf, (resource_value_t*)"ascii");
        return;

    case IDM_PRT4MPS803:
    case IDM_PRT5MPS803:
    case IDM_PRTUPMPS803:
        set_printer_res("Printer%sDriver", (idm>>4)&0xf, (resource_value_t*)"mps803");
        return;

    case IDM_PRT4TXT:
    case IDM_PRT5TXT:
    case IDM_PRTUPTXT:
        set_printer_res("Printer%sOutput", (idm>>4)&0xf, (resource_value_t*)"text");
        return;

    case IDM_PRT4GFX:
    case IDM_PRT5GFX:
    case IDM_PRTUPGFX:
        set_printer_res("Printer%sOutput", (idm>>4)&0xf, (resource_value_t*)"graphics");
        return;

#if defined __X128__ || defined __XVIC__
    case IDM_IEEE:
        toggle("IEEE488");
        return;
#endif // __X128__ || __XVIC__
    case IDM_VCACHE:
#ifdef __XCBM__
        {
            int val;
            resources_get_value("UseVicII", (resource_value_t*) &val);
            toggle(val?"VideoCache":"CrtcVideoCache");
        }
#else
        toggle(VIDEO_CACHE);
#endif
        return;
#ifdef __X128__
    case IDM_VDCVCACHE:
        toggle("VDC_VideoCache");
        return;
#endif

    case IDM_EMUID:
        toggle("EmuID");
        return;

    case IDM_REFRATEAUTO:
    case IDM_REFRATE1:
    case IDM_REFRATE2:
    case IDM_REFRATE3:
    case IDM_REFRATE4:
    case IDM_REFRATE5:
    case IDM_REFRATE6:
    case IDM_REFRATE7:
    case IDM_REFRATE8:
    case IDM_REFRATE9:
    case IDM_REFRATE10:
        resources_set_value("RefreshRate", (resource_value_t)(idm&0xf));
        return;

    case IDM_MONITOR:
        {
            //
            // open monitor dialog
            //
            //int state = !WinIsWindowVisible(hwndMonitor);

            WinActivateWindow(hwndMonitor, 1);//
             /*
            if (!state)
            {
                extern int trigger_console_exit;
                trigger_console_exit = TRUE;
                return;
            }
            */
            //
            // make sure that the build in monitor can be invoked
            //
            emulator_resume();

            //
            // trigger invokation of the build in monitor
            //
            maincpu_trigger_trap(mon_trap, NULL);
        }
        return;

        /*
         case IDM_SNAPSAVE:
         maincpu_trigger_trap(save_snapshot, (void*)hwnd);
         return;
         */
    case IDM_DATASETTE:
        datasette_dialog(hwnd);
        return;

    case IDM_DRIVE:
        drive_dialog(hwnd);
        return;

#if defined __X64__ || defined __X128__ || defined __XCBM__
    case IDM_COLOR:
        color_dialog(hwnd);
        return;

    case IDM_INTERNALPAL:
        toggle("ExternalPalette");
        return;

    case IDM_LUMINANCES:
        toggle("NewLuminances");
        return;
#endif
/*
    case IDM_EMULATOR:
        emulator_dialog(hwnd);
        return;
*/
    case IDM_SOUNDON:
        toggle("Sound");
        return;
#ifdef HAVE_RESID
    case IDM_RESID:
        toggle("SidUseResid");
        return;
    case IDM_RESIDFAST:
    case IDM_RESIDINTERPOL:
    case IDM_RESIDRESAMPLE:
        resources_set_value("SidResidSampling", (resource_value_t)(idm-IDM_RESIDFAST));
        return;

    case IDM_RESIDPASS0:
    case IDM_RESIDPASS10:
    case IDM_RESIDPASS20:
    case IDM_RESIDPASS30:
    case IDM_RESIDPASS40:
    case IDM_RESIDPASS50:
    case IDM_RESIDPASS60:
    case IDM_RESIDPASS70:
    case IDM_RESIDPASS80:
    case IDM_RESIDPASS90:
        resources_set_value("SidResidPassband", (resource_value_t)((idm&0xf) * 10));
        return;
#endif // HAVE_RESID
#if defined __X64__ || defined __X128__ || defined __XCBM__
    case IDM_SIDFILTER:
        toggle("SidFilters");
        return;

    case IDM_STEREO:
        toggle("SidStereo");
        return;

    case IDM_SC6581:
    case IDM_SC8580:
        resources_set_value("SidModel", (resource_value_t)(idm&1));
        return;
#endif // __X64__ || __X128__ || __XCBM__

    case IDM_OSOFF:
    case IDM_OS2X:
    case IDM_OS4X:
    case IDM_OS8X:
        resources_set_value("SoundOversample", (resource_value_t)(idm&0xf));
        return;

    case IDM_SYNCFLEX:
    case IDM_SYNCADJUST:
    case IDM_SYNCEXACT:
        resources_set_value("SoundSpeedAdjustment", (resource_value_t)(idm-IDM_SYNCFLEX));
        return;

    case IDM_SR8000:
        resources_set_value("SoundSampleRate", (resource_value_t)8000);
        return;
    case IDM_SR11025:
        resources_set_value("SoundSampleRate", (resource_value_t)11025);
        return;
    case IDM_SR22050:
        resources_set_value("SoundSampleRate", (resource_value_t)22050);
        return;
    case IDM_SR44100:
        resources_set_value("SoundSampleRate", (resource_value_t)44100);
        return;

    case IDM_DEVDART:
        resources_set_value("SoundDeviceName", (resource_value_t)"dart");
        return;
    case IDM_DEVDUMMY:
        resources_set_value("SoundDeviceName", (resource_value_t)"dummy");
        return;
    case IDM_DEVSID: // no sampleparams
        resources_set_value("SoundDeviceName", (resource_value_t)"dump");
        return;
    case IDM_DEVWAV: // speed
        resources_set_value("SoundDeviceName", (resource_value_t)"wav");
        return;
    case IDM_DEVRAW: // no samplepar
        resources_set_value("SoundDeviceName", (resource_value_t)"fs");
        return;

    case IDM_BUF010:
    case IDM_BUF025:
    case IDM_BUF040:
    case IDM_BUF055:
    case IDM_BUF070:
    case IDM_BUF085:
    case IDM_BUF100:
        resources_set_value("SoundBufferSize", (resource_value_t)((idm&0xff)*50));
        return;

    case IDM_VOL100:
    case IDM_VOL90:
    case IDM_VOL80:
    case IDM_VOL70:
    case IDM_VOL60:
    case IDM_VOL50:
    case IDM_VOL40:
    case IDM_VOL30:
    case IDM_VOL20:
    case IDM_VOL10:
        set_volume(10*(idm&0xf));
        return;

    case IDM_MENUBAR:
        toggle("Menubar");
        return;
/*
    case IDM_STATUSBAR:
        {
            extern void toggle_statusbar(HWND hwnd);
            toggle_statusbar(hwnd);
        }
        return;
        */
    case IDM_STRETCH1:
    case IDM_STRETCH2:
    case IDM_STRETCH3:
        resources_set_value("WindowStretchFactor", (resource_value_t)(idm&0xf));
        return;

#ifdef __XCBM__
    case IDM_CBM510:
    case IDM_CBM610:
    case IDM_CBM620:
    case IDM_CBM620P:
    case IDM_CBM710:
    case IDM_CBM720:
    case IDM_CBM720P:
        maincpu_trigger_trap(set_cbm_model, (void*)cbm_models[(idm&0xf)-1]);
        return;

    case IDM_MODEL750:
    case IDM_MODEL660:
    case IDM_MODEL650:
        resources_set_value("ModelLine", (resource_value_t)(idm&0xf));
        return;

    case IDM_RAM64:
    case IDM_RAM128:
    case IDM_RAM256:
    case IDM_RAM512:
    case IDM_RAM1024:
        resources_set_value("RamSize", (resource_value_t)((idm&0xf)<<6));
        return;

    case IDM_RAM08:
        toggle("Ram08");
        return;
    case IDM_RAM10:
        toggle("Ram1");
        return;
    case IDM_RAM20:
        toggle("Ram2");
        return;
    case IDM_RAM40:
        toggle("Ram4");
        return;
    case IDM_RAM60:
        toggle("Ram6");
        return;
    case IDM_RAMC0:
        toggle("RamC");
        return;
#endif // __XCBM__

#ifdef __XPET__
    case IDM_PET2001:
    case IDM_PET3008:
    case IDM_PET3016:
    case IDM_PET3032:
    case IDM_PET3032B:
    case IDM_PET4016:
    case IDM_PET4032:
    case IDM_PET4032B:
    case IDM_PET8032:
    case IDM_PET8096:
    case IDM_PET8296:
    case IDM_PETSUPER:
         maincpu_trigger_trap(set_pet_model, (void*)pet_models[(idm&0xf)-1]);
         return;

    case IDM_CHARSET:
        toggle("Basic1Chars");
        return;

    case IDM_EOI:
        toggle("EoiBlank");
        return;

    case IDM_ROMPATCH:
        toggle("Basic1");
        return;

    case IDM_DIAGPIN:
        toggle("DiagPin");
        return;

    case IDM_SUPERPET:
        toggle("SuperPET");
        return;

    case IDM_MAP9:
        toggle("Ram9");
        return;
    case IDM_MAPA:
        toggle("RamA");
        return;

    case IDM_PETRAM4:
    case IDM_PETRAM8:
    case IDM_PETRAM16:
    case IDM_PETRAM32:
    case IDM_PETRAM96:
    case IDM_PETRAM128:
        resources_set_value("RamSize", (resource_value_t)(idm&0xff));
        return;

    case IDM_IOMEM256:
        resources_set_value("IOSize", (resource_value_t)0x100);
        return;
    case IDM_IOMEM2K:
        resources_set_value("IOSize", (resource_value_t)0x800);
        return;

    case IDM_CRTC:
        toggle("Crtc");
        return;

    case IDM_VSDETECT:
        resources_set_value("VideoSize", (resource_value_t)0);
        return;
    case IDM_VS40:
        resources_set_value("VideoSize", (resource_value_t)40);
        return;
    case IDM_VS80:
        resources_set_value("VideoSize", (resource_value_t)80);
        return;
#endif // __XPET__

    case IDM_PAUSE:
        if (isEmulatorPaused())
            emulator_resume();
        else
            emulator_pause();
        return;

    case IDM_TRUEDRIVE: // Switch True Drive Emulatin on/off
        toggle("DriveTrueEmulation");
        return;

        //{ log output to logwindow by system
        //    char str[35];
        //    sprintf(str, "True drive emulation switched %s",
        //            toggle("DriveTrueEmulation")?"ON.":"OFF.");
        //    WinMessageBox(HWND_DESKTOP, hwnd,
        //                  str, "Drive Emulation", 0, MB_OK);
        //}
        //return;

#ifdef HAS_JOYSTICK
    case IDM_JOYSTICK:
        joystick_dialog(hwnd);
        return;
#endif

    case IDM_HLPINDEX:
        WinSendMsg(WinQueryHelpInstance(hwnd), HM_HELP_INDEX, 0, 0);
        return;

    case IDM_HLPGENERAL:
        WinSendMsg(WinQueryHelpInstance(hwnd), HM_DISPLAY_HELP, 0, 0);
        // MPFROMLONG(idPanel), MPFROMSHORT(HM_RESOURCEID)
        return;

        // case IDM_HLPUSINGHLP:
        // WinSendMsg(WinQueryHelpInstance(hwnd), HM_EXT_HELP, 0, 0);
        // return;

        // case IDM_HLPKEYBOARD:
        // WinSendMsg(WinQueryHelpInstance(hwnd), HM_KEYS_HELP, 0, 0);
        // return;

    case IDM_CMDLINE:
        cmdline_show_help((void*)hwnd);
        return;

    case IDM_ABOUT:
        about_dialog(hwnd);
        return;

    case IDM_EXIT:
        {
            extern int trigger_shutdown;
            trigger_shutdown = 1;
        }
        return;
    }
}

// --------------------------------------------------------------------------

static void WinCheckRes(HWND hwnd, USHORT id, const char *name)
{
    int val;
    resources_get_value(name, (resource_value_t*) &val);
    WinCheckMenuItem(hwnd, id, val);
}

void menu_select(HWND hwnd, USHORT item)
{
    long val;

    switch (item)
    {
#ifdef __X64__
    case IDM_FILE:
        resources_get_value("CartridgeType", (resource_value_t*) &val);
        WinEnableMenuItem(hwnd, IDM_CRTFREEZE,
                          val == CARTRIDGE_ACTION_REPLAY     ||
                          val == CARTRIDGE_KCS_POWER         ||
                          val == CARTRIDGE_FINAL_III         ||
                          val == CARTRIDGE_SUPER_SNAPSHOT    ||
                          val == CARTRIDGE_SUPER_SNAPSHOT_V5 ||
                          val == CARTRIDGE_ATOMIC_POWER      ||
                          val == CARTRIDGE_FINAL_I);
        WinEnableMenuItem(hwnd, IDM_CRTSAVEIMG, val==CARTRIDGE_EXPERT);
        WinCheckMenuItem (hwnd, IDM_CRTEXPERT,  val==CARTRIDGE_EXPERT);
        return;

    case IDM_CARTRIDGE:
        resources_get_value("CartridgeType", (resource_value_t*) &val);
        WinCheckMenuItem(hwnd, IDM_CRTGEN,     val==CARTRIDGE_CRT);
        WinCheckMenuItem(hwnd, IDM_CRTGEN8KB,  val==CARTRIDGE_GENERIC_8KB);
        WinCheckMenuItem(hwnd, IDM_CRTGEN16KB, val==CARTRIDGE_GENERIC_16KB);
        WinCheckMenuItem(hwnd, IDM_CRTACTREPL, val==CARTRIDGE_ACTION_REPLAY);
        WinCheckMenuItem(hwnd, IDM_CRTATOMPOW, val==CARTRIDGE_ATOMIC_POWER);
        WinCheckMenuItem(hwnd, IDM_CRTEPYX,    val==CARTRIDGE_EPYX_FASTLOAD);
        WinCheckMenuItem(hwnd, IDM_CRTSSSHOT,  val==CARTRIDGE_SUPER_SNAPSHOT);
        WinCheckMenuItem(hwnd, IDM_CRTSSSHOT5, val==CARTRIDGE_SUPER_SNAPSHOT_V5);
        WinCheckMenuItem(hwnd, IDM_CRTWEST,    val==CARTRIDGE_WESTERMANN);
        WinCheckMenuItem(hwnd, IDM_CRTIEEE,    val==CARTRIDGE_IEEE488);
        WinCheckMenuItem(hwnd, IDM_CRTEXPERT,  val==CARTRIDGE_EXPERT);
        return;
#endif
    case IDM_DETACH:
#ifdef __X64__
        resources_get_value("CartridgeType", (resource_value_t*) &val);
        WinEnableMenuItem(hwnd, IDM_CARTRIDGEDET, val!=CARTRIDGE_NONE);
#endif
        WinEnableMenuItem(hwnd, IDM_DETACHTAPE, tape_get_file_name());
        WinEnableMenuItem(hwnd, IDM_DETACH8,    file_system_get_disk_name(8));
        WinEnableMenuItem(hwnd, IDM_DETACH9,    file_system_get_disk_name(9));
        WinEnableMenuItem(hwnd, IDM_DETACH10,   file_system_get_disk_name(10));
        WinEnableMenuItem(hwnd, IDM_DETACH11,   file_system_get_disk_name(11));
        WinEnableMenuItem(hwnd, IDM_DETACHALL,
                          file_system_get_disk_name(8)  ||
                          file_system_get_disk_name(9)  ||
                          file_system_get_disk_name(10) ||
                          file_system_get_disk_name(11));
        return;

    case IDM_VIEW:
        WinEnableMenuItem(hwnd, IDM_LOGWIN,  hwndLog    !=NULLHANDLE);
        WinEnableMenuItem(hwnd, IDM_MONITOR, hwndMonitor!=NULLHANDLE);
#if defined __X64__ || defined __X128__
        resources_get_value("ExternalPalette", (resource_value_t*) &val);
        WinEnableMenuItem(hwnd, IDM_COLOR, !val);
#endif
#ifdef __XCBM__
        {
            int val1, val2;
            resources_get_value("ExternalPalette", (resource_value_t*) &val1);
            resources_get_value("UseVicII",        (resource_value_t*) &val2);
            WinEnableMenuItem(hwnd, IDM_COLOR, !val && val2);
        }
#endif
        return;

#if defined __X64__ || defined __X128__
        /*
         //
         // A change online is not possible yet.
         //
         case IDM_KERNALREV:
         resources_get_value("KernalRev", (resource_value_t*) &val);
         WinCheckMenuItem(hwnd, IDM_KERNALREV0,    strcmp(val, "0"));
         WinCheckMenuItem(hwnd, IDM_KERNALREV3,    strcmp(val, "3"));
         WinCheckMenuItem(hwnd, IDM_KERNALREVSX,   strcmp(val, "sx");
         WinCheckMenuItem(hwnd, IDM_KERNALREV4064, strcmp(val, "4064"));
         return;
         */
    case IDM_VIDEOSTD:
        resources_get_value("VideoStandard", (resource_value_t*) &val);
        WinCheckMenuItem(hwnd, IDM_PAL,     val==DRIVE_SYNC_PAL);
        WinCheckMenuItem(hwnd, IDM_NTSC,    val==DRIVE_SYNC_NTSC);
#ifdef __X64__
        WinCheckMenuItem(hwnd, IDM_NTSCOLD, val==DRIVE_SYNC_NTSCOLD);
#endif
        return;
#endif

    case IDM_SETUP:
#if defined VIC_II_NEED_2X || defined VIC_NEED_2X
        WinCheckRes(hwnd, IDM_VICDSIZE, "DoubleSize");
        WinCheckRes(hwnd, IDM_VICDSCAN, "DoubleScan");
#endif
#ifdef CRTC_NEED_2X
        WinCheckRes(hwnd, IDM_CRTCDSIZE, "CrtcDoubleSize");
        WinCheckRes(hwnd, IDM_CRTCDSCAN, "CrtcDoubleScan");
#endif
        WinCheckRes(hwnd, IDM_MOUSE,     "Mouse");
        WinCheckRes(hwnd, IDM_HIDEMOUSE, "HideMousePtr");
        //WinCheckRes(hwnd, IDM_PRTIEC,    "Printer4");
        //WinCheckRes(hwnd, IDM_PRTUPORT,  "PrUser");
        WinCheckRes(hwnd, IDM_EMUID,     "EmuID");
#ifdef __XCBM__
        WinCheckRes(hwnd, IDM_VCACHE,    val?"VideoCache":"CrtcVideoCache");
        resources_get_value("UseVicII", (resource_value_t*)&val);
        WinEnableMenuItem(hwnd, IDM_PALCONTROL, val);
#else
        WinCheckRes(hwnd, IDM_VCACHE,    VIDEO_CACHE);
#endif
#ifdef __X128__
        WinCheckRes(hwnd, IDM_VDCVCACHE, "VDC_VideoCache");
#endif
        WinCheckMenuItem(hwnd, IDM_PAUSE, isEmulatorPaused());
        WinCheckRes(hwnd, IDM_MENUBAR,   "Menubar");
        // WinCheckRes(hwnd, IDM_MENUBAR,   "Statusbar");
#if defined __X128__ || defined __XVIC__
        WinCheckRes(hwnd, IDM_IEEE,      "IEEE488");
#endif // __X128__ || __XVIC__
#if defined __X64__ || defined __X128__
        resources_get_value("REU", (resource_value_t*)&val);
        WinCheckMenuItem(hwnd,  IDM_REU,     val);
        WinEnableMenuItem(hwnd, IDM_REUSIZE, val);
#endif
#ifdef __XPET__
        WinCheckRes(hwnd, IDM_CHARSET,  "Basic1Chars");
        WinCheckRes(hwnd, IDM_EOI,      "EoiBlank");
        WinCheckRes(hwnd, IDM_ROMPATCH, "Basic1");
        WinCheckRes(hwnd, IDM_DIAGPIN,  "DiagPin");
        WinCheckRes(hwnd, IDM_SUPERPET, "SuperPET");
        WinCheckRes(hwnd, IDM_CRTC,     "Crtc");
#endif // __XPET__
        return;

    case IDM_PRINTER4:
    case IDM_PRINTER5:
    case IDM_PRINTERUP:
        {
            const int num = (item>>4)&0xf;

            get_printer_res("Printer%s", num, (resource_value_t*)&val);

            WinCheckMenuItem(hwnd,  IDM_PRT4IEC | (num<<4), val);
            WinEnableMenuItem(hwnd, IDM_PRT4DRV | (num<<4), val);
            WinEnableMenuItem(hwnd, IDM_PRT4OUT | (num<<4), val);
        }
        return;

    case IDM_PRT4DRV:
    case IDM_PRT5DRV:
    case IDM_PRTUPDRV:
        {
            const int num = (item>>4)&0xf;

            char *txt;

            get_printer_res("Printer%sDriver", num, (resource_value_t*)&txt);
            WinCheckMenuItem(hwnd, IDM_PRT4ASCII  | (num<<4), !strcasecmp(txt, "ascii"));
            WinCheckMenuItem(hwnd, IDM_PRT4MPS803 | (num<<4), !strcasecmp(txt, "mps803"));
        }

    case IDM_PRT4OUT:
    case IDM_PRT5OUT:
    case IDM_PRTUPOUT:
        {
            const int num = (item>>4)&0xf;

            char *txt;

            get_printer_res("Printer%sOutput", num, (resource_value_t*)&txt);
            WinCheckMenuItem(hwnd, IDM_PRT4TXT | (num<<4), !strcasecmp(txt, "text"));
            WinCheckMenuItem(hwnd, IDM_PRT4GFX | (num<<4), !strcasecmp(txt, "graphics"));

            get_printer_res("Printer%sDriver", num, (resource_value_t*)&txt);
            WinEnableMenuItem(hwnd, IDM_PRT4GFX | (num<<4), strcasecmp(txt, "ascii"));
        }

#if defined __X64__ || defined __X128__
    case IDM_REUSIZE:
        resources_get_value("REUSize", (resource_value_t*)&val);
        WinCheckMenuItem(hwnd, IDM_REU128,   val==128);
        WinCheckMenuItem(hwnd, IDM_REU256,   val==256);
        WinCheckMenuItem(hwnd, IDM_REU512,   val==512);
        WinCheckMenuItem(hwnd, IDM_REU1024,  val==1024);
        WinCheckMenuItem(hwnd, IDM_REU2048,  val==2048);
        WinCheckMenuItem(hwnd, IDM_REU4096,  val==4096);
        WinCheckMenuItem(hwnd, IDM_REU8192,  val==8192);
        WinCheckMenuItem(hwnd, IDM_REU16384, val==16384);
        return;
#endif

    case IDM_COLLISION:
        WinCheckRes(hwnd, IDM_SBCOLL, "CheckSbColl");
        WinCheckRes(hwnd, IDM_SSCOLL, "CheckSsColl");
        return;

    case IDM_REFRATE:
        resources_get_value("RefreshRate", (resource_value_t*)&val);
        {
            int i;
            for (i=0x0; i<0xb; i++)
                WinCheckMenuItem(hwnd, IDM_REFRATEAUTO|i, i==val);
        }
        return;
    case IDM_SOUNDDEV:
        {
            char *dev;
            resources_get_value("SoundDeviceName", (resource_value_t*)&dev);
            if (!dev || !dev[0])
                dev = "dart";
            WinCheckMenuItem(hwnd, IDM_DEVDART,  !strcasecmp(dev, "dart"));
            WinCheckMenuItem(hwnd, IDM_DEVSID,   !strcasecmp(dev, "dump"));
            WinCheckMenuItem(hwnd, IDM_DEVDUMMY, !strcasecmp(dev, "dummy"));
            WinCheckMenuItem(hwnd, IDM_DEVWAV,   !strcasecmp(dev, "wav"));
            WinCheckMenuItem(hwnd, IDM_DEVRAW,   !strcasecmp(dev, "fs"));
        }
        return;

    case IDM_SOUND:
        WinCheckRes(hwnd, IDM_SOUNDON, "Sound");
#ifdef HAVE_RESID
        resources_get_value("SidUseResid", (resource_value_t*)&val);
        WinCheckMenuItem (hwnd, IDM_RESID,         val);
        WinEnableMenuItem(hwnd, IDM_RESIDMETHOD,   val);
        WinEnableMenuItem(hwnd, IDM_OVERSAMPLING, !val);
        WinEnableMenuItem(hwnd, IDM_SOUNDSYNC,    !val);
#endif // HAVE_RESID
#if defined __X64__ || defined __X128__ || defined __XCBM__
        WinCheckRes(hwnd, IDM_SIDFILTER, "SidFilters");
        WinCheckRes(hwnd, IDM_STEREO, "SidStereo");
#endif // __X64__ || __X128__ || __XCBM__
        return;

#ifdef HAVE_RESID
    case IDM_RESIDMETHOD:
        resources_get_value("SidResidSampling", (resource_value_t*)&val);
        WinCheckMenuItem (hwnd, IDM_RESIDFAST,     val==0);
        WinCheckMenuItem (hwnd, IDM_RESIDINTERPOL, val==1);
        WinCheckMenuItem (hwnd, IDM_RESIDRESAMPLE, val==2);
        WinEnableMenuItem(hwnd, IDM_RESIDBAND,     val==2);
        return;

    case IDM_RESIDBAND:
        resources_get_value("SidResidPassband", (resource_value_t*)&val);
        {
            int i;
            for (i=0; i<10; i++)
                WinCheckMenuItem(hwnd, IDM_RESIDPASS0|i, i*10==val);
        }
        return;
#endif // HAVE_RESID
#if defined __X64__ || defined __X128__ || defined __XCBM__
    case IDM_SIDCHIP:
        resources_get_value("SidModel", (resource_value_t*)&val);
        WinCheckMenuItem(hwnd, IDM_SC6581, !val);
        WinCheckMenuItem(hwnd, IDM_SC8580,  val);
        return;
#endif // __X64__ || __X128__ || __XCBM__
    case IDM_SOUNDSYNC:
        resources_get_value("SoundSpeedAdjustment", (resource_value_t*)&val);
        WinCheckMenuItem(hwnd, IDM_SYNCFLEX,   val==SOUND_ADJUST_FLEXIBLE);
        WinCheckMenuItem(hwnd, IDM_SYNCADJUST, val==SOUND_ADJUST_ADJUSTING);
        WinCheckMenuItem(hwnd, IDM_SYNCEXACT,  val==SOUND_ADJUST_EXACT);
        return;

    case IDM_OVERSAMPLING:
        resources_get_value("SoundOversample", (resource_value_t*)&val);
        WinCheckMenuItem(hwnd, IDM_OSOFF, val==0);
        WinCheckMenuItem(hwnd, IDM_OS2X,  val==1);
        WinCheckMenuItem(hwnd, IDM_OS4X,  val==2);
        WinCheckMenuItem(hwnd, IDM_OS8X,  val==3);
        return;

    case IDM_SAMPLINGRATE:
        resources_get_value("SoundSampleRate", (resource_value_t*)&val);
        WinCheckMenuItem(hwnd, IDM_SR8000,  val== 8000);
        WinCheckMenuItem(hwnd, IDM_SR11025, val==11025);
        WinCheckMenuItem(hwnd, IDM_SR22050, val==22050);
        WinCheckMenuItem(hwnd, IDM_SR44100, val==44100);
        return;

    case IDM_BUFFER:
        resources_get_value("SoundBufferSize", (resource_value_t*)&val);
        WinCheckMenuItem(hwnd, IDM_BUF010, val== 100);
        WinCheckMenuItem(hwnd, IDM_BUF025, val== 250);
        WinCheckMenuItem(hwnd, IDM_BUF040, val== 400);
        WinCheckMenuItem(hwnd, IDM_BUF055, val== 550);
        WinCheckMenuItem(hwnd, IDM_BUF070, val== 700);
        WinCheckMenuItem(hwnd, IDM_BUF085, val== 850);
        WinCheckMenuItem(hwnd, IDM_BUF100, val==1000);
        return;

    case IDM_VOLUME:
        val = get_volume();
        WinCheckMenuItem(hwnd, IDM_VOL100, val==100);
        WinCheckMenuItem(hwnd, IDM_VOL90,  val== 90);
        WinCheckMenuItem(hwnd, IDM_VOL80,  val== 80);
        WinCheckMenuItem(hwnd, IDM_VOL70,  val== 70);
        WinCheckMenuItem(hwnd, IDM_VOL60,  val== 60);
        WinCheckMenuItem(hwnd, IDM_VOL50,  val== 50);
        WinCheckMenuItem(hwnd, IDM_VOL40,  val== 40);
        WinCheckMenuItem(hwnd, IDM_VOL30,  val== 30);
        WinCheckMenuItem(hwnd, IDM_VOL20,  val== 20);
        WinCheckMenuItem(hwnd, IDM_VOL10,  val== 10);
        return;

#if defined __X64__ || defined __X128__ || defined __XCBM__
    case IDM_PALCONTROL:
        resources_get_value("ExternalPalette", (resource_value_t*)&val);
        WinCheckMenuItem(hwnd, IDM_INTERNALPAL, !val);
        WinCheckRes(hwnd, IDM_LUMINANCES, "NewLuminances");
        WinEnableMenuItem(hwnd, IDM_LUMINANCES, !val);
        return;
#endif

#ifdef __X128__
    case IDM_VDCMEMORY:
        resources_get_value("VDC_64KB", (resource_value_t*)&val);
        WinCheckMenuItem(hwnd, IDM_VDC16K, val==0);
        WinCheckMenuItem(hwnd, IDM_VDC64K, val==1);
        return;

    case IDM_FUNCROM:
        WinCheckRes(hwnd, IDM_INTFUNCROM, "InternalFunctionROM");
        WinCheckRes(hwnd, IDM_EXTFUNCROM, "ExternalFunctionROM");
        return;
#endif // __X128__

    case IDM_STRETCH:
        resources_get_value("WindowStretchFactor", (resource_value_t*)&val);
        WinCheckMenuItem(hwnd, IDM_STRETCH1, val==1);
        WinCheckMenuItem(hwnd, IDM_STRETCH2, val==2);
        WinCheckMenuItem(hwnd, IDM_STRETCH3, val==3);
        return;

#ifdef __XCBM__
    case IDM_MODEL:
        {
            int i;
            for (i=0; i<7; i++)
                WinCheckMenuItem(hwnd, IDM_MODEL|(i+1),
                                 !strcmp(cbm2_get_model(), cbm_models[i]));
        }
        return;

    case IDM_MODELLINE:
        resources_get_value("ModelLine", (resource_value_t*)&val);
        WinCheckMenuItem(hwnd, IDM_MODEL750, val==0);
        WinCheckMenuItem(hwnd, IDM_MODEL660, val==1);
        WinCheckMenuItem(hwnd, IDM_MODEL650, val==2);
        return;

    case IDM_RAMSIZE:
        resources_get_value("RamSize", (resource_value_t*)&val);
        WinCheckMenuItem(hwnd, IDM_RAM64,   val==0x040); /*   64 */
        WinCheckMenuItem(hwnd, IDM_RAM128,  val==0x080); /*  128 */
        WinCheckMenuItem(hwnd, IDM_RAM256,  val==0x100); /*  256 */
        WinCheckMenuItem(hwnd, IDM_RAM512,  val==0x200); /*  512 */
        WinCheckMenuItem(hwnd, IDM_RAM1024, val==0x400); /* 1024 */
        return;

    case IDM_RAMMAPPING:
        WinCheckRes(hwnd, IDM_RAM08, "Ram08");
        WinCheckRes(hwnd, IDM_RAM10, "Ram1");
        WinCheckRes(hwnd, IDM_RAM20, "Ram2");
        WinCheckRes(hwnd, IDM_RAM40, "Ram4");
        WinCheckRes(hwnd, IDM_RAM60, "Ram6");
        WinCheckRes(hwnd, IDM_RAMC0, "RamC");
        return;
#endif // __XCBM__

#ifdef __XPET__
    case IDM_PETMODEL:
        {
            int i;
            for (i=0; i<12; i++)
                WinCheckMenuItem(hwnd, IDM_PETMODEL|(i+1),
                                 !strcmp(get_pet_model(), pet_models[i]));
        }
        return;

    case IDM_IOMEM:
        resources_get_value("IOSize", (resource_value_t*)&val);
        WinCheckMenuItem(hwnd, IDM_IOMEM256, val==0x100);
        WinCheckMenuItem(hwnd, IDM_IOMEM2K,  val==0x800);
        return;

    case IDM_RAMMAPPING:
        WinCheckRes(hwnd, IDM_MAP9, "Ram9");
        WinCheckRes(hwnd, IDM_MAPA, "RamA");
        return;

    case IDM_RAMSIZE:
        resources_get_value("RamSize", (resource_value_t*)&val);
        WinCheckMenuItem(hwnd, IDM_PETRAM4,   val==0x04); /*   4 */
        WinCheckMenuItem(hwnd, IDM_PETRAM8,   val==0x08); /*   8 */
        WinCheckMenuItem(hwnd, IDM_PETRAM16,  val==0x10); /*  16 */
        WinCheckMenuItem(hwnd, IDM_PETRAM32,  val==0x20); /*  32 */
        WinCheckMenuItem(hwnd, IDM_PETRAM96,  val==0x60); /*  96 */
        WinCheckMenuItem(hwnd, IDM_PETRAM128, val==0x80); /* 128 */
        return;

    case IDM_VIDEOSIZE:
        resources_get_value("VideoSize", (resource_value_t*)&val);
        WinCheckMenuItem(hwnd, IDM_VSDETECT, val== 0);
        WinCheckMenuItem(hwnd, IDM_VS40,     val==40);
        WinCheckMenuItem(hwnd, IDM_VS80,     val==80);
        return;
#endif // __XPET__
    }
}

// --------------------------------------------------------------------------

