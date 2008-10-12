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

#include <inttypes.h>
#include <assert.h>


int get_node_pos_by_time(struct node *node, double search_time, int32_t *x, int32_t *y)
{
	struct list_head *iter;
	struct position *coordinate;
	struct position *co_prev, *co_next;
	struct position null_position;
	int32_t w_next_x, w_prev_x;
	int32_t w_next_y, w_prev_y;
	double t_next, t_prev;
	double s_time;

	memset(&null_position, 0, sizeof(struct position));

	co_prev = co_next = NULL;

	/* iterate through position list and
	 * get previous time, following time
	 * as well as the corresponding coordinates
	 */

	co_prev = co_next = &null_position;



	__list_for_each_prev(iter, &(node->position_list)) {
		coordinate = list_entry(iter, struct position, list);
		co_next = coordinate;
		if (co_next->time - s_time >= search_time)
			break;
		co_prev = co_next;
	}

	w_next_x = co_next->x; w_next_y = co_next->y;
	w_prev_x = co_prev->x; w_prev_y = co_prev->y;
	t_next = co_next->time;
	t_prev = co_prev->time;


	/* do the calculation for time search_time:
	 *
	 *  w_next := next waypoint coordinate
	 *  w_prev := previous waypoint coordinate
	 *  t_next := next time (correspond to w_next)
	 *  t_prev := previous time (correspond to w_prev)
	 *  t_t    := searched time
	 *
	 *  both must be calculated for x and y
	 *
	 *  XXX: earlier break out:
	 *  if t_next of t_prev IS search time we
	 *  return instantly the right coordinates.
	 */

	double t_t = search_time;

	*x = ((w_next_x - w_prev_x) / ((t_next - t_prev) / t_t - t_prev)) + w_prev_x;
	*y = ((w_next_y - w_prev_y) / ((t_next - t_prev) / t_t - t_prev)) + w_prev_y;

	if (node->id == 8) {
		fprintf(stderr, "x: %d y: %d (%d:%d)\n", *x, *y, w_prev_x, w_prev_y);
	}

	return 1;
}

struct node *alloc_node(uint32_t id)
{
	struct node *n;

	n = xalloc(sizeof(struct node));
	n->id = id;

	/* 4294967295U := 10 characters */
	n->name = xalloc(10 + 5 + 1);
	snprintf(n->name, 16, "Node %u", id);

	INIT_LIST_HEAD(&n->list);
	INIT_LIST_HEAD(&n->position_list);

	return n;
}


void add_position_data_to_node(struct node *n, double t, uint32_t x, uint32_t y)
{
	assert(n);

	struct position *p;

	p = xalloc(sizeof(struct position));

	p->time = t;
	p->x = x;
	p->y = y;

	INIT_LIST_HEAD(&p->list);
	list_add(&p->list, &n->position_list);
	n->position_data_entries++;

	return;
}

void print_node_info(struct node *n)
{
	fprintf(stdout, "%s\n", n->name);
	fprintf(stdout, "position entries: %u\n", n->position_data_entries);
}



/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
