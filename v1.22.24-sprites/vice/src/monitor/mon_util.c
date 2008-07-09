/*
 * mon_util.c - Utilities for the VICE built-in monitor.
 *
 * Written by
 *  Spiro Trikaliotis <spiro.trikaliotis@gmx.de>
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

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "archdep.h"
#include "console.h"
#include "lib.h"
#include "mem.h"
#include "mon_disassemble.h"
#include "mon_util.h"
#include "monitor.h"
#include "types.h"
#include "uimon.h"


static char *             bigbuffer      = NULL;
static const unsigned int bigbuffersize  = 10000;
static unsigned int       bigbufferwrite = 0;

static void mon_buffer_alloc(void)
{
    if (!bigbuffer) {
        bigbuffer = lib_malloc(bigbuffersize + 1);
        bigbuffer[bigbuffersize] = 0;
    }
}

static int mon_buffer_flush(void)
{
    int rv = 0;

    if (bigbuffer && bigbufferwrite) {
        bigbufferwrite = 0;
        rv = uimon_out(bigbuffer);
    }

    return rv;
}

/* like strncpy, but: 
 * 1. always add a null character
 * 2. do not fill the rest of the buffer
 */
static void stringcopy_n(char *dest, const char *src, unsigned int len)
{
    while (*src && len--)
        *dest++ = *src++;

    *dest = 0;
}

static void mon_buffer_add(const char *buffer, unsigned int bufferlen)
{
    if (bigbufferwrite + bufferlen > bigbuffersize) {
        /* the buffer does not fit into bigbuffer, thus, 
           flush the buffer! */
        mon_buffer_flush();
    }

    if (bigbufferwrite + bufferlen <= bigbuffersize) {
        stringcopy_n(&bigbuffer[bigbufferwrite], buffer, bufferlen);
        bigbufferwrite += bufferlen;

        assert(bigbufferwrite <= bigbuffersize);
    }
}

static int mon_out_buffered(const char *buffer)
{
    int rv = 0;

    if (!console_log || console_log->console_cannot_output) {
        mon_buffer_alloc();
        mon_buffer_add(buffer, strlen(buffer));
    }
    else {
        rv = mon_buffer_flush();
        rv = uimon_out(buffer) || rv;
    }

    return rv;
}

int mon_out(const char *format, ...)
{
    va_list ap;
    char *buffer;
    int rc = 0;

    va_start(ap, format);
    buffer = lib_mvsprintf(format, ap);

    rc = mon_out_buffered(buffer);
    lib_free(buffer);

    if (rc < 0)
        monitor_abort();

    return rc;
}

char *mon_disassemble_with_label(MEMSPACE memspace, WORD loc, int hex,
                                 unsigned *opc_size_p, unsigned *label_p)
{
    const char *p;

    if (*label_p == 0) {
        /* process a label, if available */
        p = mon_symbol_table_lookup_name(memspace, loc);
        if (p) {
            *label_p = 1;
            *opc_size_p = 0;
            return lib_msprintf("%s:",p);
        }
    } else {
        *label_p = 0;
    }

    /* process the disassembly itself */
    p = mon_disassemble_to_string_ex(memspace, loc,
                                     mon_get_mem_val(memspace, loc),
                                     mon_get_mem_val(memspace,
                                                     (WORD)(loc + 1)),
                                     mon_get_mem_val(memspace,
                                                     (WORD)(loc + 2)),
                                     mon_get_mem_val(memspace,
                                                     (WORD)(loc + 3)),
                                     hex,
                                     opc_size_p);

    return lib_msprintf((hex ? "%04X: %s%10s" : "05u: %s%10s"), loc, p, "");
}

#ifndef __OS2__
static char *pchCommandLine = NULL;

void mon_set_command(console_t *console_log, char *command,
                     void (*pAfter)(void))
{
    pchCommandLine = command;

    if (console_log)
        console_out(console_log, "%s\n", command);

    if (pAfter)
        (*pAfter)();
}

char *uimon_in(const char *prompt)
{
    char *p = NULL;

    while (!p && !pchCommandLine) {
        /* as long as we don't have any return value... */

        /* make sure to flush the output buffer */
        mon_buffer_flush();

        /* get input from the user */
        p = uimon_get_in(&pchCommandLine, prompt);
    }

    if (pchCommandLine) {
        /* we have an "artificially" generated command line */

        if (p)
            lib_free(p);

        p = lib_stralloc(pchCommandLine);
        pchCommandLine = NULL;
    }

    /* return the command (the one or other way...) */
    return p;
}
#endif
