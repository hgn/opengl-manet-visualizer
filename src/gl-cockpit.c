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

extern struct globals *globals;

#define	COCKPIT_HEIGHT 100
#define	COCKPIT_MARGIN 0

/* FIXME: this should be initial
 * calculated */
#define	COCKPIT_FONT_HEIGHT 12

#define	COCKPIT_OVERALL_WIDTH 300

static void draw_overall_traffic_histogram(void)
{
	float x, y;

	/* draw border */
	glBegin(GL_LINES);
	glLineWidth(1.0);
	glColor4ub(0, 0, 0, 255);
	glVertex2i(COCKPIT_OVERALL_WIDTH + COCKPIT_MARGIN, globals->screen_height - COCKPIT_HEIGHT);
	glVertex2i(COCKPIT_OVERALL_WIDTH + COCKPIT_MARGIN, globals->screen_height - COCKPIT_MARGIN);
	glEnd();

	/* draw info text */
	x = COCKPIT_MARGIN + 5;
	y = globals->screen_height - COCKPIT_HEIGHT + COCKPIT_FONT_HEIGHT;
	draw_text("Cumulative Traffic Profile", &x, &y);
}

#define	NODE_ID 2

static void draw_node_specific_histrogram(void)
{
	float x, y;

	struct node *node;
	struct list_head *iter;
	struct traffic_profile *tp;
	uint32_t amount;
	char tmp_txt[64];

	uint32_t r_sim_time = floor(simulator_time());


#define	SEGMENTS 20

	int display_segments;

	if (r_sim_time < SEGMENTS) {
		display_segments = r_sim_time;
	} else {
		display_segments = 20;
	}

	int i;


	if (r_sim_time <= (uint32_t)floor(globals->scenario->end_time)) {

		node = get_node_by_id(globals->scenario, NODE_ID);
		__list_for_each(iter, &(node->traffic_profile_list)) {
			tp = list_entry(iter, struct traffic_profile, list);
			int segment = 310;
			int segment_width = 5;
			for (i = r_sim_time; i < r_sim_time + display_segments; i++) {

				amount = tp->usage[i];
				int y_ref = globals->screen_height - 15;

				glBegin(GL_QUADS);
				glColor4ub(255, 0, 0, 100);
				glVertex2i(segment, y_ref); /* Bottom Left */
				glVertex2i(segment + segment_width, y_ref); /* Bottom Right */
				glVertex2i(segment + segment_width, y_ref - (amount/ 50)); /* Top Right */
				glVertex2i(segment, y_ref - (amount / 50)); /* top left */
				glEnd();

				segment += segment_width;

			}
		}
	} else {
		amount = 0;
	}


	/* draw info text */
	x = COCKPIT_MARGIN + 5;
	y = globals->screen_height - COCKPIT_HEIGHT + COCKPIT_FONT_HEIGHT + 20;
	snprintf(tmp_txt, 63, "Traffic node %d: %u byte\n", NODE_ID, amount);
	draw_text(tmp_txt, &x, &y);
}

void draw_cockpit(void)
{

	glViewport(0, 0, globals->screen_width, globals->screen_height);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, globals->screen_width, globals->screen_height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	/* draw transparent box */
	glBegin(GL_QUADS);
	glColor4ub(200, 200, 200, 100);
	glVertex2i( COCKPIT_MARGIN, globals->screen_height - COCKPIT_MARGIN); /* Bottom Left */
	glVertex2i( globals->screen_width - COCKPIT_MARGIN, globals->screen_height - COCKPIT_MARGIN); /* Bottom Right */
	glVertex2i( globals->screen_width - COCKPIT_MARGIN, globals->screen_height - COCKPIT_HEIGHT); /* Top Right */
	glVertex2i( COCKPIT_MARGIN, globals->screen_height - COCKPIT_HEIGHT); /* top left */
	glEnd();

	/* draw a upper border */
	glBegin(GL_LINES);
	glLineWidth(1.0);
	glColor4ub(0, 0, 0, 255);
	glVertex2i(COCKPIT_MARGIN, globals->screen_height - COCKPIT_HEIGHT);
	glVertex2i(globals->screen_width - COCKPIT_MARGIN, globals->screen_height - COCKPIT_HEIGHT);
	glEnd();

	draw_overall_traffic_histogram();
	draw_node_specific_histrogram();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}


/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
