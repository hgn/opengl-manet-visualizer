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

#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>


#define	NS2_TR_EV_TYPE     0
#define	NS2_TR_TIME        2
#define	NS2_TR_NODE_ID     8
#define	NS2_TR_POS_X      10
#define	NS2_TR_POS_Y      12
#define	NS2_TR_PROTO_L    18
#define	NS2_TR_PROTO_OLSR 34

static int parse_ns2_new_wireless_position(struct scenario *s, char *data[])
{
	double mtime;
	int32_t x, y, nodeid;
	struct node *node;

	assert(s);
	assert(data);

	/* some sanity checks first */
	if (!data[NS2_TR_TIME] || !data[NS2_TR_NODE_ID] || !data[NS2_TR_POS_X] || !data[NS2_TR_POS_Y])
		return 0;

	mtime   = strtod(data[NS2_TR_TIME], NULL);
	nodeid = atoi(data[NS2_TR_NODE_ID]);
	x      = atoi(data[NS2_TR_POS_X]);
	y      = atoi(data[NS2_TR_POS_Y]);

	node = get_node_by_id(s, nodeid);
	if (!node) {
		node = alloc_node(nodeid);
		add_node_to_scenario(s, node);
	}

	/* ok, node is a valid point, add a new list entry */
	add_position_data_to_node(node, mtime, x, y);

	return 0;
}

/**
 * This routine categorize all events necessary for
 * displaying. This is concerns all packet transmission
 * like agt or olsr routing packets. As a rule of thumb:
 * detect here all what you want to display.
 */
static enum event_type packet_type(char *data[])
{
	/* it is a valid date that a line is not always
	 * of the same length -> sanity checks first */
	if (!data[NS2_TR_PROTO_L])
		return ET_UNKNOWN;


	if (!strcmp(data[NS2_TR_PROTO_L], "AGT") &&
			!strcmp(data[NS2_TR_EV_TYPE], "r")) {
		return ET_PACKET_CBR;
	} else if (!strcmp(data[NS2_TR_PROTO_OLSR], "UM-OLSR") &&
			!strcmp(data[NS2_TR_EV_TYPE], "r")) {
		return ET_PACKET_OLSR;
	} else {
		return ET_UNKNOWN;
	}
}

/* entail_cbr_in is the last interface between the trace file
 * independent event handling. entail_cbr_in() will convert
 * the values into natives one and enqueue the data to the event
 * handling routines.
 */
static void enqueue_event(struct scenario *s, char *s_time, uint32_t type, void *data)
{
	assert(s_time);
	assert(type > 0);

	add_event(s, xstrtod(s_time), type, data);
}

#define	POS_NS2_MAC_SRC 30
#define	POS_NS2_MAC_DST 32
#define	POS_NS2_SIZE    36

/**
 * map data from char ** (trace line) to the container
 * format. map_cbr_data return 1 for success and 0 in
 * a error condition (format does not match the expection)
 */
static int map_cbr_data(void *p, char *d[])
{
	struct packet_cbr *cp;

	assert(p);

	if (!d[POS_NS2_MAC_SRC] || !d[POS_NS2_MAC_DST] || !d[POS_NS2_SIZE]) {
		fprintf(stderr, "trace file seems to be corrupted\n");
		return 0;
	}

	cp = (struct packet_cbr *)p;

	cp->src       = atoi(d[POS_NS2_MAC_SRC]);
	cp->dst       = atoi(d[POS_NS2_MAC_DST]);
	cp->data_size = atoi(d[POS_NS2_SIZE]);

	return 1;
}

static int map_olsr_data(void *p, char *d[])
{
	struct packet_olsr *cp;

	assert(p);

	if (!d[POS_NS2_MAC_SRC] || !d[POS_NS2_MAC_DST] || !d[POS_NS2_SIZE]) {
		fprintf(stderr, "trace file seems to be corrupted\n");
		return 0;
	}

	cp = (struct packet_olsr *)p;

	cp->src       = atoi(d[POS_NS2_MAC_SRC]);
	cp->dst       = atoi(d[POS_NS2_MAC_DST]);
	cp->data_size = atoi(d[POS_NS2_SIZE]);

	return 1;
}


/**
 * parse ns2 new wireless trace line and enqueue important events
 * to the event list within scenario
 */
static void parse_ns2_new_wireless_event(struct scenario *s, char *data[])
{
	int ret;
	enum event_type et;
	void *packet;

	assert(s && data[NS2_TR_TIME]);

	/* categorize event */
	et = packet_type(data);

	switch (et) {
		case ET_PACKET_CBR:
			packet = alloc_packet_container(ET_PACKET_CBR);
			ret = map_cbr_data(packet, data);
			if (!ret) { /* something failed[tm] */
				free_packet_container(ET_PACKET_CBR, packet);
				return;
			}
			enqueue_event(s, data[NS2_TR_TIME], ET_PACKET_CBR, packet);
			break;
		case ET_PACKET_OLSR:
			packet = alloc_packet_container(ET_PACKET_OLSR);
			ret = map_olsr_data(packet, data);
			if (!ret) { /* something failed[tm] */
				free_packet_container(ET_PACKET_OLSR, packet);
				return;
			}
			enqueue_event(s, data[NS2_TR_TIME], ET_PACKET_OLSR, packet);
			break;
		case ET_UNKNOWN:
			/* packet is not handled or even recognized
			 * so ignore it here */
			break;
		default:
			fprintf(stderr, "Programmed error in switch/case statement\n");
			exit(EXIT_FAILURE);
	}
}

#define	IA_SIZE 128

struct scenario *parse_ns2_new_wireless_scenario(const char *file)
{
	struct scenario *scenario;
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	unsigned int first_round, line_no;


	scenario = alloc_scenario();

	fp = fopen(file, "r");
	if (fp == NULL) {
		fprintf(stderr, "Can't open trace file %s:%s\n",
				file, strerror(errno));
		exit(EXIT_FAILURE);
	}

	line_no = first_round = 1;

	while ((read = getline(&line, &len, fp)) != -1) {

		int iter;
		char *data[IA_SIZE];

		/* null out point array  */
		for (iter = 0; iter < IA_SIZE; iter++)
			data[iter] = NULL;

		/* format description:
		 * http://www.isi.edu/nsnam/ns/doc/node186.html
		 */

		iter = 0;

		data[iter++] = strtok(line, " ");

		while (iter < IA_SIZE && (data[iter++] = strtok(NULL, " ")))
			;

	   /* ok, this is a valid line, because every line in the new trace file
		* format contains the actuall position of the particular new we
		* enqueue this information here. Parsing and validy checks are done
		* by the function self */


	   parse_ns2_new_wireless_position(scenario, data);


	   /* now we save event relevant information like the reception
		* of a data packet et cetera */
	   parse_ns2_new_wireless_event(scenario, data);

	   line_no++;

	}

	if (line)
		free(line);

	fclose(fp);

	return scenario;
}




/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
