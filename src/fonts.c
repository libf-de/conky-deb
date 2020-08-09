/* Conky, a system monitor, based on torsmo
 *
 * Any original torsmo code is licensed under the BSD license
 *
 * All code written since the fork of torsmo is licensed under the GPL
 *
 * Please see COPYING for details
 *
 * Copyright (c) 2004, Hannu Saransaari and Lauri Hakkarainen
 * Copyright (c) 2005-2009 Brenden Matthews, Philip Kovacs, et. al.
 *	(see AUTHORS)
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
#include "conky.h"
#include "fonts.h"
#include "logging.h"

int selected_font = 0;
int font_count = -1;
struct font_list *fonts = NULL;

void setup_fonts(void)
{
	if ((output_methods & TO_X) == 0) {
		return;
	}
#ifdef XFT
	if (use_xft) {
		if (window.xftdraw) {
			XftDrawDestroy(window.xftdraw);
		}
		window.xftdraw = XftDrawCreate(display, window.drawable,
				DefaultVisual(display, screen), DefaultColormap(display, screen));
	}
#endif
}

int add_font(const char *data_in)
{
	if ((output_methods & TO_X) == 0) {
		return 0;
	}
	if (font_count > MAX_FONTS) {
		CRIT_ERR("you don't need that many fonts, sorry.");
	}
	font_count++;
	if (font_count == 0) {
		if (fonts != NULL) {
			free(fonts);
		}
		if ((fonts = (struct font_list *) malloc(sizeof(struct font_list)))
				== NULL) {
			CRIT_ERR("malloc");
		}
		memset(fonts, 0, sizeof(struct font_list));
	}
	fonts = realloc(fonts, (sizeof(struct font_list) * (font_count + 1)));
	memset(&fonts[font_count], 0, sizeof(struct font_list));
	if (fonts == NULL) {
		CRIT_ERR("realloc in add_font");
	}
	// must account for null terminator
	if (strlen(data_in) < DEFAULT_TEXT_BUFFER_SIZE) {
		strncpy(fonts[font_count].name, data_in, DEFAULT_TEXT_BUFFER_SIZE);
#ifdef XFT
		fonts[font_count].font_alpha = 0xffff;
#endif
	} else {
		CRIT_ERR("Oops...looks like something overflowed in add_font().");
	}
	return font_count;
}

void set_first_font(const char *data_in)
{
	if ((output_methods & TO_X) == 0) {
		return;
	}
	if (font_count < 0) {
		if ((fonts = (struct font_list *) malloc(sizeof(struct font_list)))
				== NULL) {
			CRIT_ERR("malloc");
		}
		memset(fonts, 0, sizeof(struct font_list));
		font_count++;
	}
	if (strlen(data_in) > 1) {
		strncpy(fonts[0].name, data_in, DEFAULT_TEXT_BUFFER_SIZE);
#ifdef XFT
		fonts[0].font_alpha = 0xffff;
#endif
	}
}

void free_fonts(void)
{
	int i;

	if ((output_methods & TO_X) == 0) {
		return;
	}
	for (i = 0; i <= font_count; i++) {
#ifdef XFT
		if (use_xft) {
			XftFontClose(display, fonts[i].xftfont);
			fonts[i].xftfont = 0;
		} else
#endif /* XFT */
		{
			XFreeFont(display, fonts[i].font);
			fonts[i].font = 0;
		}
	}
	free(fonts);
	fonts = 0;
	font_count = -1;
	selected_font = 0;
#ifdef XFT
	if (window.xftdraw) {
		XftDrawDestroy(window.xftdraw);
		window.xftdraw = 0;
	}
#endif /* XFT */
}

void load_fonts(void)
{
	int i;

	if ((output_methods & TO_X) == 0)
		return;
	for (i = 0; i <= font_count; i++) {
#ifdef XFT
		/* load Xft font */
		if (use_xft && fonts[i].xftfont) {
			continue;
		} else if (use_xft) {
			fonts[i].xftfont = XftFontOpenName(display, screen,
					fonts[i].name);
			if (fonts[i].xftfont) {
				continue;
			}

			ERR("can't load Xft font '%s'", fonts[i].name);
			if ((fonts[i].xftfont = XftFontOpenName(display, screen,
					"courier-12")) != NULL) {
				continue;
			}

			ERR("can't load Xft font '%s'", "courier-12");

			if ((fonts[i].font = XLoadQueryFont(display, "fixed")) == NULL) {
				CRIT_ERR("can't load font '%s'", "fixed");
			}
			use_xft = 0;

			continue;
		}
#endif
		/* load normal font */
		if (fonts[i].font || (fonts[i].font = XLoadQueryFont(display, fonts[i].name)) == NULL) {
			ERR("can't load font '%s'", fonts[i].name);
			if ((fonts[i].font = XLoadQueryFont(display, "fixed")) == NULL) {
				CRIT_ERR("can't load font '%s'", "fixed");
			}
		}
	}
}
