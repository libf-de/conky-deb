/* -*- mode: c++; c-basic-offset: 4; tab-width: 4; indent-tabs-mode: t -*-
 * vim: ts=4 sw=4 noet ai cindent syntax=cpp
 *
 * Conky, a system monitor, based on torsmo
 *
 * Any original torsmo code is licensed under the BSD license
 *
 * All code written since the fork of torsmo is licensed under the GPL
 *
 * Please see COPYING for details
 *
 * Copyright (c) 2004, Hannu Saransaari and Lauri Hakkarainen
 * Copyright (c) 2005-2012 Brenden Matthews, Philip Kovacs, et. al.
 *   (see AUTHORS)
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

#ifndef _EXEC_H
#define _EXEC_H

#include "text_object.h"

void scan_exec_arg(struct text_object *, const char *);
void scan_execi_arg(struct text_object *, const char *);
void scan_execi_bar_arg(struct text_object *, const char *);
void scan_execi_gauge_arg(struct text_object *, const char *);
void scan_execgraph_arg(struct text_object *, const char *);
void print_exec(struct text_object *, char *, int);
void print_execi(struct text_object *, char *, int);
double execbarval(struct text_object *);
double execi_barval(struct text_object *);
void free_exec(struct text_object *);
void free_execi(struct text_object *);

#endif /* _EXEC_H */
