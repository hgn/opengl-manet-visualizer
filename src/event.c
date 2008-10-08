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

static void print_event_unknown_info(struct event *e)
{
	fprintf(stdout, "event [time: %lf, type: UNKNOWN]\n", e->time);
}

struct list_head *init_active_event_list(void)
{
	struct list_head *l;

	l = xalloc(sizeof(struct list_head));

	INIT_LIST_HEAD(l);

	return l;
}

/**
 * Enqueue event to the active list
 */
void activate_event(struct list_head *a_ev_l, struct event *event)
{
	assert(a_ev_l && event);

	list_add_tail(&event->list, a_ev_l);
}


/**
 * Iterate over the activated event list and display it
 * and check if the time is older then EVENT_PERSISTENCE_TIME
 *
 * Note: this time is independent from the simulator rate
 * because the main intention is to deliever the USER a nice
 * and smooth usability experience[tm].
 *
 * So EVENT_PERSISTENCE_TIME is cleared with the current
 * accelerations rate -> EVENT_PERSISTENCE_TIME is always
 * realtime.
 */
void debug_display_events(struct list_head *a_ev_l)
{
}


/**
 * Returns the next event from the scenario list
 * of events.
 * The caller is responsible to handle the event
 * in a proper way
 */
struct event *peek_next_event(struct scenario *sc)
{
	struct event *ev;

	assert(sc);

#if 0
	if (sc->events_in_queue == 0)
		return NULL;
#endif

	sc->events_in_queue--;

	fprintf(stderr, "event in queue: %u\n", sc->events_in_queue);

	ev = list_first_entry(&sc->event_list, struct event, list);
	if (!ev)
		return NULL;

	return ev;
}


void print_event_info(struct event *e)
{
	assert(e);

	switch (e->type) {
		default:
			print_event_unknown_info(e);
			break;
	};
}



void add_event(struct scenario *s, double time, uint32_t type, void *data)
{
	struct event *e;

	e =  xalloc(sizeof(struct event));

	e->time = time;
	e->type = type;
	e->data = data;

	INIT_LIST_HEAD(&e->list);
	list_add(&e->list, &s->event_list);

	s->events_in_queue++;

	assert(s);
}


/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
