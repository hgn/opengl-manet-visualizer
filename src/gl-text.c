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


void render_node_info_string(char *string, float x, float y, float z)
{
	char *c;

	glRasterPos3f(x, y, z);

	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
	}
}

void draw_text(const char * message, const float *x, const float *y)
{
	/* raster pos sets the current raster position
	 * mapped via the modelview and projection matrices
	 */
	glColor4ub(0, 0, 0, 255);
	glRasterPos2f(*x, *y);

	/*
	 * write using bitmap and stroke chars
	 */
	while (*message) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *message);
		message++;
	}
}


/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
