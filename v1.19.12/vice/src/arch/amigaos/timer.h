/*
 * timer.h
 *
 * Written by
 *  Mathias Roslund <vice.emu@amidog.se>
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

#ifndef _TIMER_H_
#define _TIMER_H_

#include "vice.h"

#include <proto/timer.h>

#ifdef AMIGA_AROS
#include <aros/_timeval.h>
#endif

#ifndef AMIGA_OS4
void timer_gettime(struct timeval *tv);
int timer_init(void);
void timer_exit(void);
void timer_subtime(struct timeval *dt, struct timeval *st);
void timer_usleep(int us);
#else

typedef struct timer_s timer_t;

timer_t *timer_init(void);
void timer_gettime(timer_t *timer, struct timeval *tv);
void timer_subtime(timer_t *timer, struct timeval *dt, struct timeval *st);
void timer_usleep(timer_t *timer, int us);
void timer_exit(timer_t *timer);

#endif
#endif /* _TIMER_H_ */
