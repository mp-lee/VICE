/*
 * c64cpu.c - Emulation of the C64 6510 processor.
 *
 * Written by
 *  Hannu Nuotio <hannu.nuotio@tut.fi>
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

#include "monitor.h"
#include "vicii-cycle.h"

/* ------------------------------------------------------------------------- */

/* Global clock counter.  */
CLOCK maincpu_clk = 0L;

#define REWIND_FETCH_OPCODE(clock) /*clock-=2*/

/* Flag: BA low */
int maincpu_ba_low_flag = 0;

#define CLK_INC() interrupt_delay(); maincpu_clk++; maincpu_ba_low_flag = vicii_cycle()

#define CLK_ADD(clock, amount) nosuchfunction(&clock, amount)

/* Skip cycle implementation */

#define SKIP_CYCLE 0

/* opcode_t etc */

#if defined ALLOW_UNALIGNED_ACCESS

#define opcode_t DWORD

#define p0 (opcode & 0xff)
#define p1 ((opcode >> 8) & 0xff)
#define p2 (opcode >> 8)

#else /* !ALLOW_UNALIGNED_ACCESS */

#define opcode_t          \
    struct {              \
        BYTE ins;         \
        union {           \
            BYTE op8[2];  \
            WORD op16;    \
        } op;             \
    }

#define p0 (opcode.ins)
#define p2 (opcode.op.op16)

#ifdef WORDS_BIGENDIAN
#  define p1 (opcode.op.op8[1])
#else
#  define p1 (opcode.op.op8[0])
#endif

#endif /* !ALLOW_UNALIGNED_ACCESS */

/*  SET_OPCODE for traps */
#if defined ALLOW_UNALIGNED_ACCESS 
#define SET_OPCODE(o) (opcode) = o; 
#else 
#if !defined WORDS_BIGENDIAN 
#define SET_OPCODE(o)                          \
    do {                                       \
        opcode.ins = (o) & 0xff;               \
        opcode.op.op8[0] = ((o) >> 8) & 0xff;  \
        opcode.op.op8[1] = ((o) >> 16) & 0xff; \
    } while (0) 
#else 
#define SET_OPCODE(o)                          \
    do {                                       \
        opcode.ins = (o) & 0xff;               \
        opcode.op.op8[1] = ((o) >> 8) & 0xff;  \
        opcode.op.op8[0] = ((o) >> 16) & 0xff; \
    } while (0) 
#endif 
#endif 


/* FETCH_OPCODE implementation(s) */
#if defined ALLOW_UNALIGNED_ACCESS
#define FETCH_OPCODE(o) \
    do { \
        if (((int)reg_pc) < bank_limit) {                       \
            check_ba();                                         \
            o = (*((DWORD *)(bank_base + reg_pc)) & 0xffffff);  \
            CLK_INC();                                          \
            check_ba();                                         \
            CLK_INC();                                          \
            if (fetch_tab[o & 0xff]) {                          \
                check_ba();                                     \
                CLK_INC();                                      \
            }                                                   \
        } else {                                                \
            o = LOAD(reg_pc);                                   \
            CLK_INC();                                          \
            o |= LOAD(reg_pc + 1) << 8;                         \
            CLK_INC();                                          \
            if (fetch_tab[o & 0xff]) {                          \
                o |= (LOAD(reg_pc + 2) << 16);                  \
                CLK_INC();                                      \
            }                                                   \
        }                                                       \
    } while (0)

#else /* !ALLOW_UNALIGNED_ACCESS */
#define FETCH_OPCODE(o) \
    do { \
        if (((int)reg_pc) < bank_limit) {                         \
            check_ba();                                           \
            (o).ins = *(bank_base + reg_pc);                      \
            CLK_INC();                                            \
            check_ba();                                           \
            (o).op.op16 = *(bank_base + reg_pc + 1);              \
            CLK_INC();                                            \
            if (fetch_tab[(o).ins]) {                             \
                check_ba();                                       \
                (o).op.op16 |= (*(bank_base + reg_pc + 2) << 8);  \
                CLK_INC();                                        \
            }                                                     \
        } else {                                                  \
            (o).ins = LOAD(reg_pc);                               \
            CLK_INC();                                            \
            (o).op.op16 = LOAD(reg_pc + 1);                       \
            CLK_INC();                                            \
            if (fetch_tab[(o).ins]) {                             \
                (o).op.op16 |= (LOAD(reg_pc + 2) << 8);           \
                CLK_INC();                                        \
            }                                                     \
        }                                                         \
    } while (0)

#endif /* !ALLOW_UNALIGNED_ACCESS */


#include "../mainc64cpu.c"
