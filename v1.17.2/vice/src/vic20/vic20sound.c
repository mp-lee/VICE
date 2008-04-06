/*
 * vic20sound.c - Implementation of VIC20 sound code.
 *
 * Written by
 *  Rami R�s�nen <raipsu@users.sf.net>
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
#include <string.h>

#include "lib.h"
#include "maincpu.h"
#include "sound.h"
#include "types.h"
#include "vic20sound.h"

static BYTE siddata[16];

struct sound_s
{
  unsigned char div;
  unsigned char bassreg;
  unsigned char basscntr;
  unsigned char bassshift;
  unsigned char altoreg;
  unsigned char altocntr;
  unsigned char altoshift;
  unsigned char soprreg;
  unsigned char soprcntr;
  unsigned char soprshift;
  unsigned char noisereg;
  unsigned char noisecntr;
  unsigned short noiseshift;
  unsigned short noisefoo;
  unsigned char vol;
  int cycles_per_sample;
};

static struct sound_s snd;

int vic_sound_run(int cycles);

int sound_machine_calculate_samples(sound_t *psid, SWORD *pbuf, int nr,
                                    int interleave, int *delta_t)
{
    int i;

    for (i = 0; i < nr; i++) {
      pbuf[i * interleave] = (((vic_sound_run(snd.cycles_per_sample) * snd.vol) / (snd.cycles_per_sample))<<9) - 32768;
    }
    return 0;
}

sound_t *sound_machine_open(int chipno)
{
    return (sound_t*)lib_calloc(1, sizeof(sound_t));
}

int sound_machine_init(sound_t *psid, int speed, int cycles_per_sec)
{
    DWORD i;

    memset((unsigned char*)&snd, 0, sizeof(snd));

    snd.cycles_per_sample = cycles_per_sec / speed;

    for (i = 0; i < 16; i++)
        sound_machine_store(psid, (WORD)i, siddata[i]);

    return 1;
}

void sound_machine_close(sound_t *psid)
{
    lib_free(psid);
}

void vic_sound_reset(void)
{
    WORD i;

    sound_reset();
    for (i = 10; i < 15; i++)
        vic_sound_store(i, 0);
}

void vic_sound_store(WORD addr, BYTE value)
{
    addr &= 0x0f;
    siddata[addr] = value;
    sound_store(addr, value, 0);
}

int vic_sound_run(int cycles) {
  int i;
  int ret = cycles*4;  /* Initial offset */

  for (i=0;i<cycles;i++) {
    snd.div++;
    if (!(snd.div&0x3)) {
      snd.soprcntr++; snd.soprcntr &= 0x7F;
      if (snd.soprcntr == 0x7F) {
        unsigned char tmp = snd.soprshift;
        snd.soprcntr = snd.soprreg & ~0x80;
        snd.soprshift <<= 1;
        if (snd.soprreg&0x80) {
          snd.soprshift |= ((tmp^128) & 128) >> 7;
        }
      }
      snd.noisecntr++; snd.noisecntr &= 0x7F;
      if (snd.noisecntr == 0x7F) {
        unsigned short tmp = snd.noiseshift;
        snd.noisecntr = snd.noisereg & ~0x80;
        snd.noiseshift <<= 1;
        if (snd.noisereg&0x80) {
          snd.noiseshift |= ( ( (tmp&0x4000) ^ ((tmp&0x400)<<4) ^ (0x4000) ) >> (6+8) );
          if (!(snd.noiseshift & 1) && (tmp & 1))
            snd.noisefoo++;
        } else
          snd.noisefoo = 4;
      }
    }
    if (!(snd.div&0x7)) {
      snd.altocntr++; snd.basscntr &= 0x7F;
      if (snd.altocntr == 0x7F) {
        unsigned char tmp = snd.altoshift;
        snd.altocntr = snd.altoreg & ~0x80;
        snd.altoshift <<= 1;
        if (snd.altoreg&0x80) {
          snd.altoshift |= ((tmp^128) & 128) >> 7;
        }
      }
    }
    if (!(snd.div&0xF)) {
      snd.basscntr++; snd.basscntr &= 0x7F;
      if (snd.basscntr == 0x7F) {
        unsigned char tmp = snd.bassshift;
        snd.basscntr = snd.bassreg & ~0x80;
        snd.bassshift <<= 1;
        if (snd.bassreg&0x80) {
          snd.bassshift |= ((tmp^128) & 128) >> 7;
        }
      }
    }
    ret += (snd.soprshift & 1) + (snd.altoshift & 1) + (snd.bassshift & 1) + (!(snd.noisefoo & 4));
  }
  return ret;
}

void sound_machine_store(sound_t *psid, WORD addr, BYTE value)
{
    switch (addr) {
      case 0xA:
        snd.bassreg = value;
        break;
      case 0xB:
        snd.altoreg = value;
        break;
      case 0xC:
        snd.soprreg = value;
        break;
      case 0xD:
        snd.noisereg = value;
        break;
      case 0xE:
        snd.vol = value & 0x0f;
        break;
    }
}

void sound_machine_prevent_clk_overflow(sound_t *psid, CLOCK sub)
{
}

void sound_machine_reset(sound_t *psid, CLOCK cpu_clk)
{
}

BYTE sound_machine_read(sound_t *psid, WORD addr)
{
    abort();
    return 0;
}

char *sound_machine_dump_state(sound_t *psid)
{
    return lib_msprintf("#SID: clk=%d v=%d\n", maincpu_clk, snd.vol);
}

int sound_machine_cycle_based(void)
{
    return 0;
}

int sound_machine_channels(void)
{
    return 1;
}

void sound_machine_enable(int enable)
{
}
