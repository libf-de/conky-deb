/*
 * Conky, a system monitor, based on torsmo
 *
 * Any original torsmo code is licensed under the BSD license
 *
 * All code written since the fork of torsmo is licensed under the GPL
 *
 * Please see COPYING for details
 *
 * Copyright (c) 2004, Hannu Saransaari and Lauri Hakkarainen
 * Copyright (c) 2005-2007 Brenden Matthews, Philip Kovacs, et. al. (see AUTHORS)
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
 *  $Id: common.c 935 2007-08-31 02:05:02Z brenden1 $
 */

#include "conky.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/time.h>
#include <pthread.h>

struct information info;

void update_uname()
{
	uname(&info.uname_s);
}

double get_time()
{
	struct timeval tv;
	gettimeofday(&tv, 0);
	return tv.tv_sec + (tv.tv_usec / 1000000.0);
}

FILE *open_file(const char *file, int *reported)
{
	FILE *fp = fopen(file, "r");
	if (!fp) {
		if (!reported || *reported == 0) {
			ERR("can't open %s: %s", file, strerror(errno));
			if (reported)
				*reported = 1;
		}
		return 0;
	}

	return fp;
}

void variable_substitute(const char *s, char *dest, unsigned int n)
{
	while (*s && n > 1) {
		if (*s == '$') {
			s++;
			if (*s != '$') {
				char buf[256];
				const char *a, *var;
				unsigned int len;

				/* variable is either $foo or ${foo} */
				if (*s == '{') {
					s++;
					a = s;
					while (*s && *s != '}')
						s++;
				} else {
					a = s;
					while (*s && (isalnum((int) *s)
						      || *s == '_'))
						s++;
				}

				/* copy variable to buffer and look it up */
				len = (s - a > 255) ? 255 : (s - a);
				strncpy(buf, a, len);
				buf[len] = '\0';

				if (*s == '}')
					s++;

				var = getenv(buf);

				if (var) {
					/* add var to dest */
					len = strlen(var);
					if (len >= n)
						len = n - 1;
					strncpy(dest, var, len);
					dest += len;
					n -= len;
				}
				continue;
			}
		}

		*dest++ = *s++;
		n--;
	}

	*dest = '\0';
}

/* network interface stuff */

static struct net_stat netstats[16];

struct net_stat *get_net_stat(const char *dev)
{
	unsigned int i;

	if (!dev)
		return 0;

	/* find interface stat */
	for (i = 0; i < 16; i++) {
		if (netstats[i].dev && strcmp(netstats[i].dev, dev) == 0)
			return &netstats[i];
	}

	/* wasn't found? add it */
	if (i == 16) {
		for (i = 0; i < 16; i++) {
			if (netstats[i].dev == 0) {
				netstats[i].dev = strdup(dev);
				return &netstats[i];
			}
		}
	}

	CRIT_ERR("too many interfaces used (limit is 16)");
	return 0;
}

void clear_net_stats (void)
{
  memset (netstats, 0, sizeof(netstats));
}

void format_seconds(char *buf, unsigned int n, long t)
{
	if (t >= 24 * 60 * 60)	/* hours necessary when there are days? */
		snprintf(buf, n, "%ldd %ldh %ldm", t / 60 / 60 / 24,
			 (t / 60 / 60) % 24, (t / 60) % 60);
	else if (t >= 60 * 60)
		snprintf(buf, n, "%ldh %ldm", (t / 60 / 60) % 24,
			 (t / 60) % 60);
	else
		snprintf(buf, n, "%ldm %lds", t / 60, t % 60);
}

void format_seconds_short(char *buf, unsigned int n, long t)
{
	if (t >= 24 * 60 * 60)
		snprintf(buf, n, "%ldd %ldh", t / 60 / 60 / 24,
			 (t / 60 / 60) % 24);
	else if (t >= 60 * 60)
		snprintf(buf, n, "%ldh %ldm", (t / 60 / 60) % 24,
			 (t / 60) % 60);
	else
		snprintf(buf, n, "%ldm", t / 60);
}

static double last_meminfo_update;
static double last_fs_update;

unsigned long long need_mask;
#define NEED(a) ((need_mask & (1 << a)) && ((info.mask & (1 << a)) == 0))

void update_stuff()
{
	unsigned int i;
	info.mask = 0;

	if (no_buffers)
		need_mask |= 1 << INFO_BUFFERS;

	/* clear speeds and up status in case device was removed and doesn't get
	   updated */

	for (i = 0; i < 16; i++) {
		if (netstats[i].dev) {
			netstats[i].up = 0;
			netstats[i].recv_speed = 0.0;
			netstats[i].trans_speed = 0.0;
		}
	}

	prepare_update();

	if (NEED(INFO_UPTIME))
		update_uptime();

	if (NEED(INFO_PROCS))
		update_total_processes();

	if (NEED(INFO_RUN_PROCS))
		update_running_processes();

	if (NEED(INFO_CPU))
		update_cpu_usage();

	if (NEED(INFO_NET))
		update_net_stats();

	if (NEED(INFO_DISKIO))
		update_diskio();

	if (NEED(INFO_MAIL))
		update_mail_count();

#if defined(__linux__)
	if (NEED(INFO_I8K))
		update_i8k();
#endif /* __linux__ */
	
#ifdef MPD
	if (NEED(INFO_MPD)) {
		if (!mpd_timed_thread) {
			clear_mpd_stats(&info); 
			mpd_timed_thread = timed_thread_create((void*)update_mpd, (void*) NULL, update_interval * 1000000);
			if (!mpd_timed_thread) {
				ERR("Failed to create MPD thread");
			}
			timed_thread_register(mpd_timed_thread, &mpd_timed_thread);
		}
	}
#endif

#ifdef XMMS2
	if (NEED(INFO_XMMS2))
		update_xmms2();
#endif

#ifdef AUDACIOUS
	if (NEED(INFO_AUDACIOUS))
		update_audacious();
#endif

#ifdef BMPX
	if (NEED(INFO_BMPX))
                update_bmpx();
#endif

	if (NEED(INFO_LOADAVG))
		update_load_average();


	if ((NEED(INFO_MEM) || NEED(INFO_BUFFERS) || NEED(INFO_TOP)) &&
	    current_update_time - last_meminfo_update > 6.9) {
		update_meminfo();
		if (no_buffers) {
			info.mem -= info.bufmem;
		}
		last_meminfo_update = current_update_time;
	}

	if (NEED(INFO_TOP))
		update_top();

	/* update_fs_stat() won't do anything if there aren't fs -things */
	if (NEED(INFO_FS) && current_update_time - last_fs_update > 12.9) {
		update_fs_stats();
		last_fs_update = current_update_time;
	}
#ifdef TCP_PORT_MONITOR
	if (NEED(INFO_TCP_PORT_MONITOR))
		update_tcp_port_monitor_collection( info.p_tcp_port_monitor_collection );
#endif
	if (NEED(INFO_ENTROPY))
		update_entropy();
}

int round_to_int(float f)
{
    int intval = (int)f;
    double delta = f - intval;
    if (!(delta < 0.5)) {
        ++intval;
    }

    return intval;
}
