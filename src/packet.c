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

static void *alloc_packet_container_olsr(void)
{
	return xalloc(sizeof(struct packet_olsr));
}

static void *alloc_packet_container_cbr(void)
{
	return xalloc(sizeof(struct packet_cbr));
}

void *alloc_packet_container(uint32_t type)
{
	assert(type > 0);

	switch (type) {
		case ET_PACKET_OLSR:
			return alloc_packet_container_olsr();
			break;
		case ET_PACKET_CBR:
			return alloc_packet_container_cbr();
			break;
		default:
			die("Programmed error - packet type not known\n");
			break;
	}

	return NULL; /* should never happen */
}

static void free_packet_container_cbr(void *p)
{
	free(p);
}

static void free_packet_container_olsr(void *p)
{
	free(p);
}

void free_packet_container(uint32_t t, void *p)
{
	assert(p);
	assert(t > 0);

	switch (t) {
		case ET_PACKET_OLSR:
			return free_packet_container_cbr(p);
			break;
		case ET_PACKET_CBR:
			return free_packet_container_olsr(p);
			break;
		default:
			die("Programmed error - packet type not known\n");
			break;
	}
}


/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
