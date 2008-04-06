/*
 * soundmmos2.c - Implementation of the MMOS2 sound device
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

#include "vice.h"

#define INCL_DOSPROFILE
#define INCL_DOSPROCESS
#define INCL_DOSSEMAPHORES
#include <os2.h>
#define INCL_MCIOS2
#include <os2me.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sound.h"
#include "log.h"
#include "utils.h"

#include "sounddrv.h"

typedef struct _playlist
{
    ULONG ulCommand;
    ULONG ulOperandOne;
    ULONG ulOperandTwo;
    ULONG ulOperandThree;
} PLAYLIST;

PLAYLIST Playlist[3]={
    {DATA_OPERATION,   0, 0, 0},
    {BRANCH_OPERATION, 0, 0, 0},  // jump to instruction 0
    {EXIT_OPERATION,   0, 0, 0}   // maybe not needed
};

static USHORT usSoundDevID;
//ULONG  ulSamplesPerSec;
//USHORT usBitsPerSample;

SWORD *dataBuf;

static void mmos2_close(void)
{   // free: vorher sound anhalten!
    int rc;
    // MCI_WAIT hangs???
    log_message(LOG_DEFAULT, "soundmmos2.c: mmos2_close");
    rc=mciSendCommand(usSoundDevID, MCI_STOP, MCI_WAIT, NULL, 0);
    if (rc != MCIERR_SUCCESS) sound_err(rc, "Error stopping MMOS2 Playback (MCI_STOP)");
    log_message(LOG_DEFAULT, "soundmmos2.c: MCI stopped");
    rc=mciSendCommand(usSoundDevID, MCI_CLOSE, MCI_WAIT, NULL, 0);
    if (rc != MCIERR_SUCCESS) sound_err(rc, "Error closing MMOS2 Waveform Audio Device (MCI_CLOSE)");
    log_message(LOG_DEFAULT, "soundmmos2.c: MCI closed");
    free(dataBuf);
    log_message(LOG_DEFAULT, "soundmmos2.c: dataBuf freed");
}

int pos, frag_size, frag_numbers;
MCI_OPEN_PARMS mciOpen;

static int mmos2_init(const char *param, int *speed,
                      int *fragsize, int *fragnr, int *stereo)
{
    int rc, i;
    QWORD qwTmrTime1, qwTmrTime2;
    MCI_WAVE_SET_PARMS mciSet; // fragnr = refreshs / buffer; (gerundet)

    /* No stereo capability. */
    *stereo = 0;

    if (usSoundDevID) return 1;

    mciOpen.pszDeviceType  = MCI_DEVTYPE_WAVEFORM_AUDIO_NAME;
    mciOpen.hwndCallback   = (HWND) NULL;
    mciOpen.pszAlias       = (CHAR) NULL;
    mciOpen.pszElementName = (PSZ)  Playlist;
    rc=mciSendCommand(0, MCI_OPEN,
                      MCI_WAIT|
                      MCI_OPEN_PLAYLIST|
                      MCI_OPEN_SHAREABLE,
                      &mciOpen, 0);

    if (rc != MCIERR_SUCCESS) return sound_err(rc, "Error open MMOS2 Waveform Audio Device (MCI_OPEN)");

    usSoundDevID = mciOpen.usDeviceID;

    mciSet.ulSamplesPerSec = *speed;
    mciSet.usBitsPerSample = sizeof(SWORD)*8;
    rc=mciSendCommand(usSoundDevID, MCI_SET,
                      MCI_WAIT|
                      MCI_WAVE_SET_SAMPLESPERSEC|
                      MCI_WAVE_SET_BITSPERSAMPLE,
                      &mciSet, 0);

    if (rc != MCIERR_SUCCESS) return sound_err(rc, "Error setting Sample Rate or Saples Per Sec (MCI_SET)");

    //    atexit(mmos2_close);
    pos=*fragsize*0;//3;//3*4;//14
    frag_size    = *fragsize;
    frag_numbers = *fragnr;//*3; //(*speed)/(*fragsize)*bufsize;  // entspricht 0.349s
    log_message(LOG_DEFAULT, "soundmmos2.c: speed %i Hz", *speed);
//    log_message(LOG_DEFAULT, "soundmmos2.c: bufsize %1.4f s", bufsize);
    log_message(LOG_DEFAULT, "soundmmos2.c: fragsize %i byte", *fragsize);
    log_message(LOG_DEFAULT, "soundmmos2.c: fragnumbers %i", frag_numbers);
    dataBuf=(short*)xcalloc(1,*fragsize*frag_numbers*sizeof(SWORD)*2);
    Playlist[0].ulOperandOne   = (ULONG)dataBuf;
    Playlist[0].ulOperandTwo   = *fragsize*frag_numbers*sizeof(SWORD);
    Playlist[0].ulOperandThree = 0;
//    rc=mciSendCommand(usSoundDevID, MCI_PLAY, 0, &mciOpen, 0);
//    if (rc != MCIERR_SUCCESS) return sound_err(rc, "MCI_PLAY");
    return 0;
} // lautstaerke einstellen und speichern!!

signed long old;

static int mmos2_write(SWORD *pbuf, size_t nr)
{   // maximale differenz anschauen!
    static ULONG first;

    memcpy(dataBuf+pos, pbuf, nr*sizeof(SWORD)); // dst, src, cnt
    if (!first) {
        int rc=mciSendCommand(usSoundDevID, MCI_PLAY, 0, &mciOpen, 0);
        if (rc != MCIERR_SUCCESS) return sound_err(rc, "MCI_PLAY");
        first=TRUE;
    }
    pos += nr;  // fragsize
    pos %= (frag_size*frag_numbers);
    return 0;
}
    /*MCI_STATUS_PARMS mciStat;
    mciStat.ulItem = MCI_STATUS_POSITION; //_IN_TRACK; here: the same?

    mciSendCommand(usSoundDevID, MCI_STATUS,
                      MCI_WAIT|
                      MCI_STATUS_ITEM,
                      &mciStat, 0);
                      mciStat.ulReturn = (double)mciStat.ulReturn*(44100/3000);     // milliseconds
                      */

/* return number of samples free in the kernel buffer at the moment */
static int mmos2_bufferspace(void) // nr of samples in buffer
{
    return 0;
}

static int mmos2_suspend(void)
{
    int rc;
    //    mmlog("pausing",0);
    rc=mciSendCommand(usSoundDevID, MCI_PAUSE, MCI_WAIT, NULL, 0);
    if (rc != MCIERR_SUCCESS) sound_err(rc, "Error pausing MMOS2 Playback (MCI_PAUSE)");
    //    mmlog("paused",0);
    return 0;
}

static int mmos2_resume(void)
{
    int rc;
    //    mmlog("resuming",0);
    rc=mciSendCommand(usSoundDevID, MCI_RESUME, MCI_WAIT, NULL, 0);
    if (rc != MCIERR_SUCCESS) sound_err(rc, "Error resuming MMOS2 Playback (MCI_RESUME)");
    //    mmlog("resumed",0);
    return 0;
}

static sound_device_t mmos2_device =
{
    "mmos2",
    mmos2_init,         // init
    mmos2_write,        // write
    NULL,               // dump
    NULL,               // flush
    NULL,//mmos2_bufferspace, // bufferspace
    mmos2_close,        // close
    NULL,//mmos2_suspend,      // suspend
    NULL//mmos2_resume        // resume
};

int sound_init_mmos2_device(void)
{
    return sound_register_device(&mmos2_device);
}
