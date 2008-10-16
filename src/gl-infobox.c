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

#define MESSAGE	"MANET Network Visualizer"

extern struct globals *globals;

int frame,time,timebase=0;
char fps[64];


void draw_infobox(void)
{
	char *msg = MESSAGE;
	char s_time_str[64];
	float x, y;

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
	glColor4ub(200, 200, 200, 150);
	glVertex2i( 5, 50); /* Bottom Left */
	glVertex2i( 200, 50); /* Bottom Right */
	glVertex2i( 200, 5); /* Top Right */
	glVertex2i( 5, 5); /* top left */
	glEnd();

	/* draw border */

	snprintf(s_time_str, sizeof(s_time_str), "Simulation Time: %.2lfs", simulator_time());

	x = 7.0, y = 17.0;
	draw_text(msg, &x, &y);
	y = 45.0;
	draw_text(fps, &x, &y);
	y = 31.0;
	draw_text(s_time_str, &x, &y);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(fps, "FPS: %4.2f", frame * 1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
}


/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
