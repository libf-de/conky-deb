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
#ifndef _SPECIALS_H
#define _SPECIALS_H

/* special stuff in text_buffer */

#define SPECIAL_CHAR '\x01'

/* why 256? cause an array of more then 256 doubles seems excessive,
 * and who needs that kind of precision anyway? */
#define MAX_GRAPH_DEPTH 256

//don't use spaces in LOGGRAPH or NORMGRAPH if you change them
#define LOGGRAPH "log"
#define NORMGRAPH "normal"

enum special_types {
	HORIZONTAL_LINE,
	STIPPLED_HR,
	BAR,
	FG,
	BG,
	OUTLINE,
	ALIGNR,
	ALIGNC,
	GAUGE,
	GRAPH,
	OFFSET,
	VOFFSET,
	FONT,
	GOTO,
	TAB,
};

struct special_t {
	int type;
	short height;
	short width;
	long arg;
	double *graph;
	double graph_scale;
	short show_scale;
	int graph_width;
	int scaled;
	unsigned long first_colour;	// for graph gradient
	unsigned long last_colour;
	short font_added;
};

/* direct access to the registered specials (FIXME: bad encapsulation) */
extern struct special_t *specials;
extern int special_count;

extern int default_bar_width;
extern int default_bar_height;
#ifdef X11
extern int default_graph_width;
extern int default_graph_height;
extern int default_gauge_width;
extern int default_gauge_height;
#endif

/* max number of specials allowed (TODO: use linked list instead) */
extern int max_specials;

#ifdef X11
/* scanning special arguments */
const char *scan_gauge(const char *, int *, int *);
const char *scan_bar(const char *, int *, int *);
char *scan_font(const char *);
char *scan_graph(const char *, int *, int *, unsigned int *,
                 unsigned int *, unsigned int *, char *);

/* printing specials */
void new_gauge(char *, int, int, int);
void new_bar(char *, int, int, int);
void new_font(char *, char *);
void new_graph(char *, int, int, unsigned int,
               unsigned int, double, int, int, char);
void new_hr(char *, int);
void new_stippled_hr(char *, int, int);
#endif
void new_fg(char *, long);
void new_bg(char *, long);
void new_outline(char *, long);
void new_offset(char *, long);
void new_voffset(char *, long);
void new_alignr(char *, long);
void new_alignc(char *, long);
void new_goto(char *, long);
void new_tab(char *, int, int);

#endif /* _SPECIALS_H */
