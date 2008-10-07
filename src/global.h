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

#ifndef NV_GLOBAL_H
#define	NV_GLOBAL_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <math.h>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "list.h"

struct image {
	uint32_t size_x;
	uint32_t size_y;
	uint8_t *data;
};

enum image_file_type {
	BMP_TYPE = 1,
	TIFF_TYPE
};

#define	TRACE_FILE_FAKE 1
#define	TRACE_FILE_NS2  2

#define	TRACE_FILE_DEFAULT TRACE_FILE_FAKE

enum event_type {
	ET_CBR_IN = 1,
	ET_UNKNOWN
};

struct position {
	double time;
	int32_t x;
	int32_t y;
	struct list_head list;
};

struct node {
	uint32_t id;
	char *name;

	uint32_t position_data_entries;
	struct list_head position_list;

	struct list_head list;
};

struct event {
	double time;
	uint32_t type;
	void *data;
	struct list_head list;
};


struct scenario {
	uint32_t different_nodes;
	double start_time;
	double end_time;

	uint32_t map_x_min, map_x_max;
	uint32_t map_y_min, map_y_max;

	struct list_head node_list;
	struct list_head event_list;
};


/* terrain.c */
void draw_terrain(void);
int init_terrain(void);

/* display */
void reshape(int, int);
void display(void);
void special(int, int, int);
void keyboard(unsigned char, int, int);

/* util.c */
void *xalloc(size_t);
void die(const char *);

/* texture.c */
struct image *load_image(char *, enum image_file_type);
void free_image(struct image *);

/* skybox.c */
int init_skybox(void);
void draw_skybox(float, float);

/* ground.c */
int init_ground(void);
void draw_ground(void);

/* mouse.c */
void mouseMovement(int, int);





/* trace_parser.c */
struct scenario *parse_offline_scenario(int, const char *);
void print_trace_file_info(struct scenario *);

int get_node_pos_by_time(uint32_t, double, int32_t *, int32_t *);

void print_event_info(struct event *);
struct list_head *init_active_event_list(void);
void debug_display_events(struct list_head *);

/* scenario.c */
struct scenario *alloc_scenario(void);
struct node *get_node_by_id(struct scenario *, uint32_t);

void print_nodes_info(struct scenario *);

/* node.c */
void print_node_info(struct node *);





//3D studio loader
#define MAX_VERTICES 16000 // Max number of vertices (for each object)
#define MAX_POLYGONS 16000 // Max number of polygons (for each object)


// Our vertex type
typedef struct{
	float x,y,z;
} vertex_type;

// The polygon (triangle), 3 numbers that aim 3 vertices
typedef struct{
	int a,b,c;
}polygon_type;

// The mapcoord type, 2 texture coordinates for each vertex
typedef struct{
	float u,v;
}mapcoord_type;

// The object type
typedef struct {
	char name[20];

	int vertices_qty;
	int polygons_qty;

	vertex_type vertex[MAX_VERTICES]; 
	polygon_type polygon[MAX_POLYGONS];
	mapcoord_type mapcoord[MAX_VERTICES];
	int id_texture;
} obj_type, *obj_type_ptr;


enum {							//for the shadow rendering
	X, Y, Z, W
};
enum {
	A, B, C, D
};

char load_model_3ds(obj_type_ptr, char *);

#endif /* NV_GLOBAL_H */



/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
