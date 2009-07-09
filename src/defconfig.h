#ifndef __DEFCONFIG_H
#define __DEFCONFIG_H

#define defconfig { \
"# Conky, a system monitor, based on torsmo\n", \
"#\n", \
"# Any original torsmo code is licensed under the BSD license\n", \
"#\n", \
"# All code written since the fork of torsmo is licensed under the GPL\n", \
"#\n", \
"# Please see COPYING for details\n", \
"#\n", \
"# Copyright (c) 2004, Hannu Saransaari and Lauri Hakkarainen\n", \
"# Copyright (c) 2005-2009 Brenden Matthews, Philip Kovacs, et. al. (see AUTHORS)\n", \
"# All rights reserved.\n", \
"#\n", \
"# This program is free software: you can redistribute it and/or modify\n", \
"# it under the terms of the GNU General Public License as published by\n", \
"# the Free Software Foundation, either version 3 of the License, or\n", \
"# (at your option) any later version.\n", \
"#\n", \
"# This program is distributed in the hope that it will be useful,\n", \
"# but WITHOUT ANY WARRANTY; without even the implied warranty of\n", \
"# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n", \
"# GNU General Public License for more details.\n", \
"# You should have received a copy of the GNU General Public License\n", \
"# along with this program.  If not, see <http://www.gnu.org/licenses/>.\n", \
"#\n", \
"\n", \
"background no\n", \
"cpu_avg_samples 2\n", \
"net_avg_samples 2\n", \
"no_buffers yes\n", \
"out_to_console no\n", \
"out_to_stderr no\n", \
"update_interval 1.0\n", \
"uppercase no\n", \
"use_spacer none\n", \
"\n", \
"TEXT\n", \
"${scroll 16 $nodename - $sysname $kernel on $machine | }\n", \
"Uptime: $uptime\n", \
"Frequency (in MHz): $freq\n", \
"Frequency (in GHz): $freq_g\n", \
"RAM Usage: $mem/$memmax - $memperc%\n", \
"Swap Usage: $swap/$swapmax - $swapperc%\n", \
"CPU Usage: $cpu%\n", \
"Processes: $processes  Running: $running_processes\n", \
"File systems:\n", \
" / ${fs_used /}/${fs_size /}\n", \
"Networking:\n", \
"Up: ${upspeed eth0}  - Down: ${downspeed eth0}\n", \
"Name              PID   CPU%   MEM%\n", \
" ${top name 1} ${top pid 1} ${top cpu 1} ${top mem 1}\n", \
" ${top name 2} ${top pid 2} ${top cpu 2} ${top mem 2}\n", \
" ${top name 3} ${top pid 3} ${top cpu 3} ${top mem 3}\n", \
" ${top name 4} ${top pid 4} ${top cpu 4} ${top mem 4}\n", \
NULL }

#define print_defconfig() { \
	const char **__sp, *__s[] = defconfig; \
	for (__sp = __s; *__sp; __sp++) \
		printf("%s", *__sp); \
}

#endif /* __DEFCONFIG_H */
