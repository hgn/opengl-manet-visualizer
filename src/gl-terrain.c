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

static GLuint ground_textures[1];

#define TERRAIN_WIDTH 50


extern struct scenario *scenario;

static GLuint terrain_inner_list;
static GLuint terrain_outer_list;

int init_terrain(void)
{
	int i;
	struct image *image;
	float max_x, max_z;

	assert(scenario);

	/* calculate the coordinates of the map */
	max_x = (float)scenario->map_x_max - scenario->map_x_min;
	max_z = (float)scenario->map_y_max - scenario->map_y_min;

	/* prevent a null map -> if all nodes are static on the
	   same position
	*/
	max_x = 50;
	max_z = 50;

	/* load textures required textures */
	image = load_image("textures/ground-01/grass.bmp", BMP_TYPE);
	if (!image)
		die("Can't load texture for skydome");

	glGenTextures(1, &ground_textures[0]);

	glBindTexture(GL_TEXTURE_2D, ground_textures[0]);	// 2d texture (x and y size)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// scale linearly when image bigger than texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// scale linearly when image smalled than texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image->size_x, image->size_y, 0,
		     GL_RGB, GL_UNSIGNED_BYTE, image->data);

	free_image(image);


	terrain_inner_list = glGenLists(1);

	glNewList(terrain_inner_list, GL_COMPILE);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ground_textures[0]);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	/* draw standard scenario plate - all movments are done there */
	glBegin(GL_QUADS);

	glNormal3f(0.0f, 1.0f, 0.0f);

	glTexCoord2f(0.0f, 100.f);
	glVertex3f(0.0f, -1.0f, 0.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, max_z);

	glTexCoord2f(100.f, 0.0f);
	glVertex3f(max_x, -1.0f, max_z);

	glTexCoord2f(100.f, 100.f);
	glVertex3f(max_x, -1.0f, 0.0f);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glEndList();



	int x, y, n, a, b;

	int max_border = max_x > max_z ? max_x : max_z;


	float **terrain = xalloc(TERRAIN_WIDTH * sizeof(float *));
	for (i = 0; i < TERRAIN_WIDTH; i++) {
		terrain[i] = xalloc(max_border * sizeof(float));
	}





    // Generate random height points
    for(x = 0; x < TERRAIN_WIDTH; x++) {
        for(y = 0; y < max_border; y++){
            terrain[x][y] = (rand()%100) * .01;
        }
    }

#if 0

    // Smooth terrain (reduce jaggedness)
	for(n = 0; n<2; n++){
		for(x = 0; x<WIDTH; x++){
			for(y = 0; y<HEIGHT; y++){
				for(a = -1; a < 2; a++){
					for(b = -1; b < 2; b++){
						if(x+a>=0 && x+a<WIDTH && y+b>=0 && y+b<HEIGHT)
						{terrain[x][y]+=terrain[x+a][y+b];}
					}
				}
				terrain[x][y] *= .075;
			}
		}
	}

#endif

	terrain_outer_list = glGenLists(1);
	glNewList(terrain_outer_list, GL_COMPILE);

    glBegin(GL_QUADS);
    for(x = 0; x< TERRAIN_WIDTH - 1; x++){
        for(y = 0; y< max_border - 1; y++){

			glNormal3f(0.0f, 1.0f, 0.0f);
            glVertex3f(x+0+max_x, terrain[x+0][y+1], y+1+max_z);
            glVertex3f(x+1+max_x, terrain[x+1][y+1], y+1+max_z);
            glVertex3f(x+1+max_x, terrain[x+1][y+0], y+0+max_z);
            glVertex3f(x+0+max_x, terrain[x+0][y+0], y+0+max_z);
        }
    }

	glEnd();

	glEndList();


	free(terrain);



	return 1;
}


void draw_terrain(void)
{

#if 0
	int x, y;


#endif

	glCallList(terrain_inner_list);
	glCallList(terrain_outer_list);


}



/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
