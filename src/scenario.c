/*
** Copyright (C) 2006 - Hagen Paul Pfeifer <hagen@jauu.net>
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

struct scenario *alloc_scenario(void)
{
	struct scenario *scenario;

	scenario = xalloc(sizeof(struct scenario));

	INIT_LIST_HEAD(&scenario->node_list);
	INIT_LIST_HEAD(&scenario->event_list);

	scenario->events_in_queue = 0;

	return scenario;
}

int add_node_to_scenario(struct scenario *s, struct node *n)
{
	assert(s && n);

	INIT_LIST_HEAD(&n->list);
	list_add(&n->list, &s->node_list);

	return 0;
}

void print_nodes_info(struct scenario *s)
{
	struct list_head *iter;
	struct node *node_ptr;

	assert(s);

	__list_for_each(iter, &(s->node_list)) {
		node_ptr = list_entry(iter, struct node, list);
		print_node_info(node_ptr);
	}
}

void debug_display_nodes_coordinates_at_time(struct scenario *s, double s_time)
{
	struct list_head *iter;
	struct node *node_ptr;
	int32_t x, y;

	assert(s);

	__list_for_each(iter, &(s->node_list)) {
		node_ptr = list_entry(iter, struct node, list);
		get_node_pos_by_time(node_ptr->id, s_time, &x, &y);
		fprintf(stdout, "node %u [x: %d y:%d]\n", node_ptr->id, x, y);
	}

}

void print_all_events_info(struct scenario *s)
{
	struct list_head *iter;
	struct event *ev_ptr;

	assert(s);

	__list_for_each(iter, &(s->event_list)) {
		ev_ptr = list_entry(iter, struct event, list);
		print_event_info(ev_ptr);
	}
}

struct node *get_node_by_id(struct scenario *s, uint32_t id)
{
	struct list_head *iter;
	struct node *node_ptr;

	assert(s);

	__list_for_each(iter, &(s->node_list)) {
		node_ptr = list_entry(iter, struct node, list);
		if (node_ptr->id == id)
			return node_ptr;
	}

	return NULL;
}


/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
