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

struct opts {
	char *tracefile;
	int traceformat;
};

#define	TRACE_FILE_FAKE 1
#define	TRACE_FILE_NS2  2

#define	TRACE_FILE_DEFAULT TRACE_FILE_FAKE

enum event_type {
	ET_UNKNOWN = 1,
	ET_PACKET_OLSR,
	ET_PACKET_CBR
};

struct position {
	double time;
	int32_t x;
	int32_t y;
	struct list_head list;
};

struct traffic_profile {
	uint32_t type; /* OLSR, CBR, ... */
	uint32_t *usage; /* list per second in bit/s */
	struct list_head list;
};

struct node {
	uint32_t id;
	char *name;
	float color[3];

	uint32_t position_data_entries;
	struct list_head position_list;
	struct list_head traffic_profile_list;

	struct list_head list;
};

/* definition for ET_PACKET_OLSR */
struct packet_olsr {
	uint32_t src, dst, data_size;
};

/* definition for ET_PACKET_CBR */
struct packet_cbr {
	uint32_t src, dst, data_size;
};

/* the next define defines the amount of time
 * in seconds for how a active event (transmission
 * of a packet) should be active. This is neccessary
 * to display the event a little bit longer then
 * 0.0001 seconds
 */
#define	EVENT_PERSISTENCE_TIME 0.25

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

	uint32_t events_in_queue;
};


struct globals {
	uint32_t screen_width, screen_height;
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
inline double xstrtod(const char *);

/* getopt.c */
struct opts *parse_cl_options(int, char **);

/* time.c */
double simulator_time(void);
void setup_simulator_ref_time(void);

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

/* event.c */
void activate_event(struct list_head *, struct event *);
struct event *peek_next_event(struct scenario *);
void add_event(struct scenario *, double, uint32_t, void *);

/* trace-parser.c */
struct scenario *parse_offline_scenario(int, const char *);

/* trace-parser-ns2.c */
struct scenario *parse_ns2_new_wireless_scenario(const char *);


void print_event_info(struct event *);
struct list_head *init_active_event_list(void);
void cli_display_events(struct list_head *);

/* scenario.c */
struct scenario *alloc_scenario(void);
struct node *get_node_by_id(struct scenario *, uint32_t);
int add_node_to_scenario(struct scenario *, struct node *);

void print_nodes_info(struct scenario *);

/* node.c */
void print_node_info(struct node *);
void init_nodes(struct scenario *);
int get_node_pos_by_time(struct node *, double, float *, float *);
struct node *alloc_node(uint32_t);
void add_position_data_to_node(struct node *, double, uint32_t, uint32_t);

/* gl-nodes.c */
int init_gl_nodes(void);

/* gl-node.c */
void map_draw_nodes(void);

/* gl-text.c */
void render_node_info_string(char *, float, float, float);
void draw_text(const char *, const float *, const float *);

/* packet.c */
void *alloc_packet_container(uint32_t);
void free_packet_container(uint32_t, void *);

/* gl-packet.c */
void visualize_packets(void);

/* gl-cockpit.c */
void draw_cockpit(void);


/* gl-infobox.c */
void draw_infobox(void);




/* Max number of vertices */
#define MAX_VERTICES 16000
/* max number of polygons */
#define MAX_POLYGONS 16000


/* Our vertex type */
typedef struct{
	float x,y,z;
} vertex_type;

/* The polygon (triangle), 3 numbers that aim 3 vertices */
typedef struct{
	int a,b,c;
}polygon_type;

/* The mapcoord type, 2 texture coordinates for each vertex */
typedef struct{
	float u,v;
}mapcoord_type;

/* The object type */
typedef struct {
	char name[20];

	int vertices_qty;
	int polygons_qty;

	vertex_type vertex[MAX_VERTICES];
	polygon_type polygon[MAX_POLYGONS];
	mapcoord_type mapcoord[MAX_VERTICES];
	int id_texture;
} obj_type, *obj_type_ptr;


enum {							/* for the shadow rendering */
	X, Y, Z, W
};
enum {
	A, B, C, D
};

char load_model_3ds(obj_type_ptr, char *);

#endif /* NV_GLOBAL_H */



/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
