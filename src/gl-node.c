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


extern struct scenario *scenario;

obj_type object;				//3DS object, in this case a vintage gun

static void calculateNormal( float *coord1, float *coord2, float *coord3 )
{
	/* calculate normal of polygon */
	float va[3], vb[3], vr[3], val;
	va[0] = coord1[0] - coord2[0];
	va[1] = coord1[1] - coord2[1];
	va[2] = coord1[2] - coord2[2];

	vb[0] = coord1[0] - coord3[0];
	vb[1] = coord1[1] - coord3[1];
	vb[2] = coord1[2] - coord3[2];

	vr[0] = va[1] * vb[2] - vb[1] * va[2];
	vr[1] = vb[0] * va[2] - va[0] * vb[2];
	vr[2] = va[0] * vb[1] - vb[0] * va[1];

	/* get length of normal */
	val = sqrt( vr[0]*vr[0] + vr[1]*vr[1] + vr[2]*vr[2] );

	/* specify normalized normal */
	glNormal3f( vr[0]/val, vr[1]/val, vr[2]/val );
}

int init_nodes(void)
{
	load_model_3ds(&object, "/usr/share/manet-visualizer/3d-models/misc/sphere.3ds");

	return 1;
}

void map_draw_nodes(void)
{
	/* iterate over all nodes */
	struct list_head *iter;
	struct node *node_ptr;
	double s_time;
	int32_t x, y;

	assert(scenario);

	s_time = simulator_time();

	x = y = 0;

	__list_for_each(iter, &(scenario->node_list)) {
		node_ptr = list_entry(iter, struct node, list);
		get_node_pos_by_time(node_ptr, s_time, &x, &y);
		fprintf(stderr, "node %d [x:%d,y:%d]\n", node_ptr->id, x, y);
	}
}


void draw_nodes(void)
{
	int l_index;

	glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
	for (l_index=0;l_index<object.polygons_qty;l_index++)
	{
		//prepare tp calculate lighting normals
		float vert1[3],vert2[3],vert3[3];

		vert1[0] = object.vertex[ object.polygon[l_index].a ].x;
		vert1[1] = object.vertex[ object.polygon[l_index].a ].y;
		vert1[2] = object.vertex[ object.polygon[l_index].a ].z;

		vert2[0] = object.vertex[ object.polygon[l_index].b ].x;
		vert2[1] = object.vertex[ object.polygon[l_index].b ].y;
		vert2[2] = object.vertex[ object.polygon[l_index].b ].z;

		vert3[0] = object.vertex[ object.polygon[l_index].c ].x;
		vert3[1] = object.vertex[ object.polygon[l_index].c ].y;
		vert3[2] = object.vertex[ object.polygon[l_index].c ].z;

		calculateNormal(vert1,vert2,vert3);

		//----------------- FIRST VERTEX -----------------
		// Texture coordinates of the first vertex
		//glTexCoord2f( object.mapcoord[ object.polygon[l_index].a ].u,
		//              object.mapcoord[ object.polygon[l_index].a ].v);
		// Coordinates of the first vertex
		glVertex3f( object.vertex[ object.polygon[l_index].a ].x/10,
				object.vertex[ object.polygon[l_index].a ].y/10,
				object.vertex[ object.polygon[l_index].a ].z/10); //Vertex definition

		//----------------- SECOND VERTEX -----------------
		// Texture coordinates of the second vertex
		// glTexCoord2f( object.mapcoord[ object.polygon[l_index].b ].u,
		//              object.mapcoord[ object.polygon[l_index].b ].v);
		// Coordinates of the second vertex
		glVertex3f( object.vertex[ object.polygon[l_index].b ].x/10,
				object.vertex[ object.polygon[l_index].b ].y/10,
				object.vertex[ object.polygon[l_index].b ].z/10);

		//----------------- THIRD VERTEX -----------------
		// Texture coordinates of the third vertex
		// glTexCoord2f( object.mapcoord[ object.polygon[l_index].c ].u,
		//              object.mapcoord[ object.polygon[l_index].c ].v);
		// Coordinates of the Third vertex
		glVertex3f( object.vertex[ object.polygon[l_index].c ].x/10,
				object.vertex[ object.polygon[l_index].c ].y/10,
				object.vertex[ object.polygon[l_index].c ].z/10);
	}
	glEnd();
}



/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
