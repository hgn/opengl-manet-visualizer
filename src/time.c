/*
** Copyright (C) 2008 - Hagen Paul Pfeifer <hagen@jauu.net>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "global.h"
#include <assert.h>
#include <sys/time.h>

struct timeval reference_time;

void setup_simulator_ref_time(void)
{
	gettimeofday(&reference_time, NULL);
}



double simulator_time(void)
{
	double ret;
	struct timeval current_time;

	gettimeofday(&current_time, NULL);

	ret = (((double)reference_time.tv_sec - current_time.tv_sec) * 1000000000) +
		((double)reference_time.tv_usec - current_time.tv_usec);

	return ret;
}


/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
