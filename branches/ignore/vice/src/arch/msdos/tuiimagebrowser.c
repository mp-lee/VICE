/*
 * tuiimagebrowser.c - Explore disk/tape image contents.
 *
 * Written by
 *  Ettore Perazzoli (ettore@comm2000.it)
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

#include <pc.h>
#include <keys.h>

#include "imagecontents.h"
#include "tui.h"
#include "utils.h"

#include "tuiimagebrowser.h"

#define WIDTH  35
#define HEIGHT 17

static void display_item(int x, int y, int width, int offset,
                         image_contents_file_list_t *element,
                         int is_selected)
{
    int x1, y1, j;

    y1 = y + offset;

    if (is_selected)
        tui_set_attr(MENU_FORE, MENU_HIGHLIGHT, 0);
    else
        tui_set_attr(MENU_FORE, MENU_BACK, 0);
    tui_display(x, y1, 6, "%-6d", element->size);

    x1 = x + 6;
    tui_put_char(x1++, y1, '\"');
    for (j = 0; element->name[j] != 0; j++, x1++)
        tui_put_char(x1, y1, element->name[j]);
    tui_put_char(x1++, y1, '\"');

    for (; x1 < x + width - IMAGE_CONTENTS_TYPE_LEN; x1++)
        tui_put_char(x1, y1, ' ');
    tui_display(x1, y1, IMAGE_CONTENTS_TYPE_LEN, "%s", element->type);
}

static void update(int x, int y, int width, int height,
                   image_contents_file_list_t *first,
                   image_contents_file_list_t *selected)
{
    image_contents_file_list_t *p;
    int i;

    for (p = first, i = 0; p != NULL && i < height; p = p->next, i++)
        display_item(x, y, width, i, p, p == selected);

    tui_set_attr(MENU_FORE, MENU_BACK, 0);
    for (; i < height; i++)
        tui_hline(x, y + i, ' ', width);
}

/* FIXME: colors.  */
char *tui_image_browser(const char *filename,
                        image_contents_t *(*contents_func)(const char *))
{
    image_contents_t *contents;
    tui_area_t backing_store = NULL;
    image_contents_file_list_t *first, *current;
    int first_number, current_number;
    int x, y;
    int real_width, real_height;
    int real_x, real_y;
    int need_update;

    contents = contents_func(filename);
    if (contents == NULL) {
        tui_error("Invalid image");
        return NULL;
    }

    x = CENTER_X(WIDTH);
    y = CENTER_Y(HEIGHT);
    tui_display_window(x, y,
                       WIDTH, HEIGHT,
                       MESSAGE_FORE, MESSAGE_BACK,
                       NULL, &backing_store);

    real_width = WIDTH - 4;
    real_height = HEIGHT - 4;
    real_x = x + 2;
    real_y = y + 3;

    tui_set_attr(MESSAGE_FORE, MESSAGE_BACK, 0);
    if (*contents->id != 0) 
        tui_display(real_x, y + 1, real_width,
                    "\"%s\", %s", contents->name, contents->id);
    else
        tui_display(real_x, y + 1, real_width,
                    "\"%s\"", contents->name);
    tui_hline(real_x - 1, y + 2, 0xcd, real_width + 2);

    if (contents->blocks_free >= 0) {
        real_height -= 2;
        tui_display(real_x, y + HEIGHT - 2, real_width,
                    "%d blocks free.", contents->blocks_free);
        tui_hline(real_x - 1, y + HEIGHT - 3, 0xcd, real_width + 2);
    }

    first_number = current_number = 0;
    first = current = contents->file_list;

    if (current != NULL) {
        need_update = 1;
    } else {
        need_update = 0;
        tui_display(real_x, real_y, real_width, "(Empty image)");
    }

    while (1) {
        int key;

        if (need_update) {
            update(real_x, real_y, real_width, real_height, first, current);
            need_update = 0;
        } else if (current != NULL) {
            display_item(real_x, real_y, real_width,
                         current_number - first_number,
                         current, 1);
        }

        key = getkey();

        if (current != NULL)
            display_item(real_x, real_y, real_width,
                         current_number - first_number,
                         current, 0);

        if (key == K_Escape) {
            tui_area_put(backing_store, x, y);
            tui_area_free(backing_store);
            image_contents_free(contents);
            return NULL;
        } else if (current != NULL) {
            switch (key) {
              case K_Return:
                {
                    char *retval;

                    tui_area_put(backing_store, x, y);
                    tui_area_free(backing_store);
                    retval = stralloc(current->name);
                    image_contents_free(contents);
                    return retval;
                }
              case K_Up:
              case K_Left:
                if (current->prev != NULL) {
                    current = current->prev;
                    current_number--;
                    if (current_number < first_number) {
                        first = first->prev;
                        first_number--;
                        need_update = 1;
                    }
                }
                break;
              case K_Down:
              case K_Right:
                if (current->next != NULL) {
                    current = current->next;
                    current_number++;
                    if (current_number - first_number + 1 > real_height) {
                        first = first->next;
                        first_number++;
                        need_update = 1;
                    }
                }
                break;
            }
        }
    }
} 
