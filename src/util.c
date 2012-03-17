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
#include <errno.h>

void die(const char *msg)
{
	fprintf(stderr, "ERROR: %s!\n", msg);
	exit(EXIT_FAILURE);
}

void die_sys(const char *msg)
{
	fprintf(stderr, "ERROR: %s:%s\n", msg, strerror(errno));
	exit(EXIT_FAILURE);
}

void *xalloc(size_t size)
{
	void *ptr;

	ptr = malloc(size);
	if (!ptr)
		die_sys("OOM");

	memset(ptr, 0, size);

	return ptr;
}

double xstrtod(const char *c_digit)
{
	double ret;
	char *endptr;

	errno = 0;
	ret = strtod(c_digit, &endptr);
	if (*endptr != '\0' || errno != 0)
		return 0.0;

	return ret;
}

/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
