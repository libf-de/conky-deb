/* Conky, a system monitor, based on torsmo
 *
 * Please see COPYING for details
 *
 * Copyright (c) 2005-2009 Brenden Matthews, et. al.
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "imlib2.h"
#include "config.h"
#include "logging.h"

#include <Imlib2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct image_list_s {
	char name[DEFAULT_TEXT_BUFFER_SIZE];
	Imlib_Image image;
	int x, y, w, h;
	int wh_set;
	struct image_list_s *next;
};

struct image_list_s *image_list_start, *image_list_end;

/* areas to update */
Imlib_Updates updates, current_update;
/* our virtual framebuffer image we draw into */
Imlib_Image buffer, image;

static int cache_size_set = 0;

#define DEFAULT_CACHE_SIZE 4096 * 1024 /* default cache size for loaded images */

void cimlib_set_cache_size(long size)
{
	imlib_set_cache_size(size);
	cache_size_set = 1;
}

void cimlib_cleanup(void)
{
	struct image_list_s *cur = image_list_start, *last = NULL;
	while (cur) {
		last = cur;
		cur = last->next;
		free(last);
	}
	image_list_start = image_list_end = NULL;
}

void cimlib_init(Display *display, Window drawable, Visual *visual, Colormap colourmap)
{
	image_list_start = image_list_end = NULL;
	if (!cache_size_set) cimlib_set_cache_size(DEFAULT_CACHE_SIZE);
	/* set the maximum number of colors to allocate for 8bpp and less to 256 */
	imlib_set_color_usage(256);
	/* dither for depths < 24bpp */
	imlib_context_set_dither(1);
	/* set the display , visual, colormap and drawable we are using */
	imlib_context_set_display(display);
	imlib_context_set_visual(visual);
	imlib_context_set_colormap(colourmap);
	imlib_context_set_drawable(drawable);
}

void cimlib_add_image(const char *args)
{
	struct image_list_s *cur = NULL;
	char *tmp;

	cur = malloc(sizeof(struct image_list_s));
	memset(cur, 0, sizeof(struct image_list_s));

	if (!sscanf(args, "%1024s", cur->name)) {
		ERR("Invalid args for $image.  Format is: '<path to image> (-p x,y) (-s WxH)' (got '%s')", args);
	}
	// now we check for optional args
	tmp = strstr(args, "-p ");
	if (tmp) {
		tmp += 3;
		sscanf(tmp, "%i,%i", &cur->x, &cur->y);
	}
	tmp = strstr(args, "-s ");
	if (tmp) {
		tmp += 3;
		if (sscanf(tmp, "%ix%i", &cur->w, &cur->h)) {
			cur->wh_set = 1;
		}
	}

	if (image_list_end) {
		image_list_end->next = cur;
		image_list_end = cur;
	} else {
		image_list_start = image_list_end = cur;
	}
}

static void cimlib_draw_image(struct image_list_s *cur, int *clip_x, int *clip_y, int *clip_x2, int *clip_y2)
{
	image = imlib_load_image(cur->name);
	if (image) {
		int w, h;
		DBGP("Drawing image '%s' at (%i,%i) scaled to %ix%i", cur->name, cur->x, cur->y, cur->w, cur->h);
		imlib_context_set_image(image);
		/* turn alpha channel on */
		imlib_image_set_has_alpha(1);
		w = imlib_image_get_width();
		h = imlib_image_get_height();
		if (!cur->wh_set) {
			cur->w = w;
			cur->h = h;
		}
		imlib_context_set_image(buffer);
		imlib_blend_image_onto_image(image, 1, 0, 0, w, h,
				cur->x, cur->y, cur->w, cur->h);
		imlib_context_set_image(image);
		imlib_free_image();
		if (cur->x < *clip_x) *clip_x = cur->x;
		if (cur->y < *clip_y) *clip_y = cur->y;
		if (cur->x + cur->w > *clip_x2) *clip_x2 = cur->x + cur->w;
		if (cur->y + cur->h > *clip_y2) *clip_y2 = cur->y + cur->h;
	} else {
		ERR("Unable to load image '%s'", cur->name);
	}
}

static void cimlib_draw_all(int *clip_x, int *clip_y, int *clip_x2, int *clip_y2)
{
	struct image_list_s *cur = image_list_start;
	while (cur) {
		cimlib_draw_image(cur, clip_x, clip_y, clip_x2, clip_y2);
		cur = cur->next;
	}
}

void cimlib_render(int x, int y, int width, int height)
{
	int clip_x = INT_MAX, clip_y = INT_MAX;
	int clip_x2 = 0, clip_y2 = 0;

	if (!image_list_start) return; /* are we actually drawing anything? */
	/* take all the little rectangles to redraw and merge them into
	 * something sane for rendering */
	buffer = imlib_create_image(width, height);
	/* clear our buffer */
	imlib_context_set_image(buffer);
	imlib_image_clear();
	/* we can blend stuff now */
	imlib_context_set_blend(1);
	/* turn alpha channel on */
	imlib_image_set_has_alpha(1);

	cimlib_draw_all(&clip_x, &clip_y, &clip_x2, &clip_y2);

	/* set the buffer image as our current image */
	imlib_context_set_image(buffer);

	/* setup our clip rect */
	if (clip_x == INT_MAX) clip_x = 0;
	if (clip_y == INT_MAX) clip_y = 0;

	/* render the image at 0, 0 */
	imlib_render_image_part_on_drawable_at_size(clip_x, clip_y, clip_x2 - clip_x,
			clip_y2 - clip_y, x + clip_x, y + clip_y, clip_x2 - clip_x,
			clip_y2 - clip_y);
	/* don't need that temporary buffer image anymore */
	imlib_free_image();
}

