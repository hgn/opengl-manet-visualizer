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

static GLuint skydome_textures[6];

int init_skybox(void)
{
	int i;
	struct image *image[6];

	GLenum cube_map_target[6] = {
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB,
		GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB
	};

	/* load textures required textures */
	image[0] = load_image("/usr/share/manet-visualizer/textures/skydome-02/xn.bmp", BMP_TYPE);
	if (!image[0])
		die("Can't load texture for skydome");

	image[1] = load_image("/usr/share/manet-visualizer/textures/skydome-02/xp.bmp", BMP_TYPE);
	if (!image[1])
		die("Can't load texture for skydome");

	image[2] = load_image("/usr/share/manet-visualizer/textures/skydome-02/yn.bmp", BMP_TYPE);
	if (!image[2])
		die("Can't load texture for skydome");

	image[3] = load_image("/usr/share/manet-visualizer/textures/skydome-02/yp.bmp", BMP_TYPE);
	if (!image[3])
		die("Can't load texture for skydome");

	image[4] = load_image("/usr/share/manet-visualizer/textures/skydome-02/zn.bmp", BMP_TYPE);
	if (!image[4])
		die("Can't load texture for skydome");

	image[5] = load_image("/usr/share/manet-visualizer/textures/skydome-02/zp.bmp", BMP_TYPE);
	if (!image[5])
		die("Can't load texture for skydome");

	glGenTextures(1, skydome_textures);

	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, *skydome_textures);

	for (i = 0; i < 6; i++) {
		glTexImage2D(cube_map_target[i],
				0,
				3,
				image[i]->size_x,
				image[i]->size_y,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				image[i]->data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP );

	free_image(image[0]); free_image(image[1]);
	free_image(image[2]); free_image(image[3]);
	free_image(image[4]); free_image(image[5]);

	return 1;
}


void draw_skybox(float camera_yaw, float camera_pitch)
{
	float t = 1.0f;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	glDisable(GL_LIGHTING);

	glDepthMask(GL_FALSE);

	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, *skydome_textures);

	glPushMatrix();
	glLoadIdentity();
	glRotatef( camera_pitch, 1.0f, 0.0f, 0.0f );
	glRotatef( camera_yaw, 0.0f, 1.0f, 0.0f );


	/* x negative */
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord3f(-t,-t,-t); glVertex3f(-t,-t,-t);
		glTexCoord3f(-t,t,-t); glVertex3f(-t,t,-t);
		glTexCoord3f(-t,-t,t); glVertex3f(-t,-t,t);
		glTexCoord3f(-t,t,t); glVertex3f(-t,t,t);
	glEnd();

	/* x positive */
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord3f(t, -t,-t); glVertex3f(t,-t,-t);
		glTexCoord3f(t,-t,t); glVertex3f(t,-t,t);
		glTexCoord3f(t,t,-t); glVertex3f(t,t,-t);
		glTexCoord3f(t,t,t); glVertex3f(t,t,t);
	glEnd();

	/* y negative */
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord3f(-t,-t,-t); glVertex3f(-t,-t,-t);
		glTexCoord3f(-t,-t,t); glVertex3f(-t,-t,t);
		glTexCoord3f(t, -t,-t); glVertex3f(t,-t,-t);
		glTexCoord3f(t,-t,t); glVertex3f(t,-t,t);
	glEnd();

	/* y positive */
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord3f(-t,t,-t); glVertex3f(-t,t,-t);
		glTexCoord3f(t,t,-t); glVertex3f(t,t,-t);
		glTexCoord3f(-t,t,t); glVertex3f(-t,t,t);
		glTexCoord3f(t,t,t); glVertex3f(t,t,t);
	glEnd();

	/* z negative */
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord3f(-t,-t,-t); glVertex3f(-t,-t,-t);
		glTexCoord3f(t, -t,-t); glVertex3f(t,-t,-t);
		glTexCoord3f(-t,t,-t); glVertex3f(-t,t,-t);
		glTexCoord3f(t,t,-t); glVertex3f(t,t,-t);
	glEnd();

	/* z positive */
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord3f(-t,-t,t); glVertex3f(-t,-t,t);
		glTexCoord3f(-t,t,t); glVertex3f(-t,t,t);
		glTexCoord3f(t,-t,t); glVertex3f(t,-t,t);
		glTexCoord3f(t,t,t); glVertex3f(t,t,t);
	glEnd();

	glPopMatrix();

	glDepthMask(GL_TRUE);

	// Réinitialisation des états OpenGL
	glDisable(GL_TEXTURE_CUBE_MAP_ARB); 
	glEnable(GL_LIGHTING);
}



/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
