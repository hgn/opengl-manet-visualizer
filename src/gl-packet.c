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
#include "list.h"

extern struct scenario *scenario;
extern struct list_head *a_ev_l;
extern uint32_t active_events;

static void node_position(uint32_t node_id, float *x, float *y)
{
	struct list_head *iter;
	struct node *node_ptr;

	double s_time = simulator_time();

	__list_for_each(iter, &(scenario->node_list)) {
		node_ptr = list_entry(iter, struct node, list);
		if (node_ptr->id == node_id) {
			get_node_pos_by_time(node_ptr, s_time, x, y);
			return;
		}
	}
}


static void draw_packet_olsr(struct event *e)
{
	float src_x, src_y;
	float dst_x, dst_y;

	struct packet_olsr *pc;

	pc = (struct packet_olsr *) e->data;

	/* calculate node position for source
	 * and destination of packet */
	node_position(pc->src, &src_x, &src_y);
	node_position(pc->dst, &dst_x, &dst_y);

	glBegin(GL_LINES);
	glLineWidth(4.0);
	glNormal3f(1.0f, 1.0f, 1.0f);
	glColor3f(1.0f, 1.0f, 0.0f );
	glVertex3f(src_x / 20, 0.01f, src_y / 20);
	glVertex3f(dst_x / 20, 0.01f, dst_y / 20);
	glEnd();
}


static void draw_packet_cbr(struct event *e)
{
	float src_x, src_y;
	float dst_x, dst_y;

	struct packet_cbr *pc;

	pc = (struct packet_cbr *) e->data;

	/* calculate node position for source
	 * and destination of packet */
	node_position(pc->src, &src_x, &src_y);
	node_position(pc->dst, &dst_x, &dst_y);

	glBegin(GL_LINES);
	glLineWidth(4.0);
	glNormal3f(1.0f, 1.0f, 1.0f);
	glColor3f(1.0f, 0.0f, 0.0f );
	glVertex3f(src_x / 20, 0.01f, src_y / 20);
	glVertex3f(dst_x / 20, 0.01f, dst_y / 20);
	glEnd();
}


/**
 * draw packet determine the type of the packet, determine
 * the source and destination and visualize the packet
 */
static void draw_packet(struct event *e)
{
	assert(e);

	switch (e->type) {
		case ET_PACKET_CBR:
			draw_packet_cbr(e);
			break;
		case ET_PACKET_OLSR:
			draw_packet_olsr(e);
			break;
		case ET_UNKNOWN:
			return; /* no visualization neccessary */
			break;
		default:
			die("Programmed error - event type unknown\n");
			break;

	}

	return;
}

/**
 * visualize_packets is called from the main visualization
 * routine
 */
void visualize_packets(void)
{
	struct list_head *iter, *tmp;
	struct event *event_ptr;

	assert(a_ev_l);

	if (active_events == 0)
		return;

	list_for_each_safe(iter, tmp, a_ev_l) {
		event_ptr = list_entry(iter, struct event, list);
		draw_packet(event_ptr);
	}
}


/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
