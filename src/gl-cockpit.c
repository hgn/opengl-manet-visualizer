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

extern struct globals globals;

#define	COCKPIT_HEIGHT 100
#define	COCKPIT_MARGIN 0

void draw_cockpit(void)
{

	glViewport(0, 0, globals.screen_width, globals.screen_height);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, globals.screen_width, globals.screen_height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	/* draw transparent box */
	glBegin(GL_QUADS);
	glColor4ub(200, 200, 200, 100);
	glVertex2i( COCKPIT_MARGIN, globals.screen_height - COCKPIT_MARGIN); /* Bottom Left */
	glVertex2i( globals.screen_width - COCKPIT_MARGIN, globals.screen_height - COCKPIT_MARGIN); /* Bottom Right */
	glVertex2i( globals.screen_width - COCKPIT_MARGIN, globals.screen_height - COCKPIT_HEIGHT); /* Top Right */
	glVertex2i( COCKPIT_MARGIN, globals.screen_height - COCKPIT_HEIGHT); /* top left */
	glEnd();

	/* draw a upper border */
	glBegin(GL_LINES);
	glLineWidth(1.0);
	glColor4ub(0, 0, 0, 255);
	glVertex2i(COCKPIT_MARGIN, globals.screen_height - COCKPIT_HEIGHT);
	glVertex2i(globals.screen_width - COCKPIT_MARGIN, globals.screen_height - COCKPIT_HEIGHT);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}


/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
