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

#define	NS2_TRACE_MAX_DATA 128

#define	NS2_TR_EV_TYPE     0
#define	NS2_TR_TIME        2
#define	NS2_TR_NODE_ID     8
#define	NS2_TR_POS_X      10
#define	NS2_TR_POS_Y      12
#define	NS2_TR_PROTO_L    18
#define	NS2_TR_PROTO_OLSR 34

#define	NS2_AGT_DATA_AMOUNT  36

extern struct globals *globals;

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

	mtime   = xstrtod(data[NS2_TR_TIME]);
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
static enum event_type packet_type_rx(char *data[])
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

/**
 * The same as packet_type_rx but for sender side
 */
static enum event_type packet_type_tx(char *data[])
{
	/* it is a valid date that a line is not always
	 * of the same length -> sanity checks first */
	if (!data[NS2_TR_PROTO_L])
		return ET_UNKNOWN;


	if (!strcmp(data[NS2_TR_PROTO_L], "AGT") &&
			!strcmp(data[NS2_TR_EV_TYPE], "t")) {
		return ET_PACKET_CBR;
	} else if (!strcmp(data[NS2_TR_PROTO_OLSR], "UM-OLSR") &&
			!strcmp(data[NS2_TR_EV_TYPE], "t")) {
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

#define POS_NS2_OLSR_SRC 26
#define POS_NS2_OLSR_DST 8

static int map_olsr_data(void *p, char *d[])
{
	struct packet_olsr *cp;

	assert(p);

	if (!d[POS_NS2_MAC_SRC] || !d[POS_NS2_MAC_DST] || !d[POS_NS2_SIZE]) {
		fprintf(stderr, "trace file seems to be corrupted\n");
		return 0;
	}

	cp = (struct packet_olsr *)p;

	cp->src       = atoi(d[POS_NS2_OLSR_SRC]);
	cp->dst       = atoi(d[POS_NS2_OLSR_DST]);
	cp->data_size = atoi(d[POS_NS2_SIZE]);

	return 1;
}

static void account_simulation_end_time(struct scenario *s, char *data[])
{
	assert(s && data[NS2_TR_TIME]);

	s->end_time = xstrtod(data[NS2_TR_TIME]);
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
	et = packet_type_rx(data);

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

static uint32_t *alloc_traffic_profile(enum event_type et, uint32_t seconds)
{
	struct traffic_profile *tp;

	assert(seconds > 0);

	tp = xalloc(sizeof(struct traffic_profile));

	tp->usage = xalloc(sizeof(uint32_t) * seconds);
	tp->type = et;

	INIT_LIST_HEAD(&tp->list);

	return tp;
}

#define	POS_NI 8
struct node *cbr_rx_origin(struct scenario *scenario, char *data[])
{
	int node_id;
	struct node *n;

	assert(data[POS_NI]);

	node_id = atoi(data[POS_NI]);

	n = get_node_by_id(scenario, node_id);
	if (!n) {
		fprintf(stderr, "Node %d not in database, seems like a programming error\n", n);
		exit(EXIT_FAILURE);
	}

	return n;
}

struct node *olsr_rx_origin(char *data[])
{
}

struct traffic_profile *get_traffic_profile_for_node(struct scenario *scenario,
		struct node *node, enum event_type et)
{
	struct list_head *iter;
	struct traffic_profile *tp;

	assert(node);
	assert((uint32_t)floor(scenario->end_time) > 0);

	__list_for_each(iter, &(node->traffic_profile_list)) {
		tp = list_entry(iter, struct traffic_profile, list);
		if (tp->type == et) {
			return tp;
		}
	}

	tp = alloc_traffic_profile(et, (uint32_t)ceil(scenario->end_time));
	list_add(&tp->list, &node->traffic_profile_list);

	return tp;
}

static void create_rx_traffic_profile(struct scenario *scenario, char  *data[])
{
	enum event_type et;
	struct node *node;
	struct traffic_profile *tp;

	et = packet_type_rx(data);

	switch (et) {
		case ET_PACKET_CBR:
			node = cbr_rx_origin(scenario, data);
			tp = get_traffic_profile_for_node(scenario, node, ET_PACKET_CBR);
			fprintf(stderr, "%p %u %u\n", tp, (uint32_t)floor(xstrtod(data[NS2_TR_TIME])),
					atoi(data[NS2_AGT_DATA_AMOUNT]));
			tp->usage[(uint32_t)floor(xstrtod(data[NS2_TR_TIME]))] += atoi(data[NS2_AGT_DATA_AMOUNT]);
			break;
		case ET_PACKET_OLSR:
			node = olsr_rx_origin(data);
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

static void create_tx_traffic_profile(struct scenario *scenario, char  *data[])
{
}


static void calculate_traffic_profile(struct scenario *scenario, FILE *fp)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	unsigned int first_round, line_no;

	while ((read = getline(&line, &len, fp)) != -1) {

		int iter;
		char *data[NS2_TRACE_MAX_DATA];

		/* null out point array  */
		for (iter = 0; iter < NS2_TRACE_MAX_DATA; iter++)
			data[iter] = NULL;

		iter = 0;

		data[iter++] = strtok(line, " ");

		while (iter < NS2_TRACE_MAX_DATA && (data[iter++] = strtok(NULL, " ")))
			;

		create_rx_traffic_profile(scenario, data);

		line_no++;

	}

	if (line)
		free(line);
}


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
		char *data[NS2_TRACE_MAX_DATA];

		/* null out point array  */
		for (iter = 0; iter < NS2_TRACE_MAX_DATA; iter++)
			data[iter] = NULL;

		/* format description:
		 * http://www.isi.edu/nsnam/ns/doc/node186.html
		 */

		iter = 0;

		data[iter++] = strtok(line, " ");

		while (iter < NS2_TRACE_MAX_DATA && (data[iter++] = strtok(NULL, " ")))
			;

	   /* ok, this is a valid line, because every line in the new trace file
		* format contains the actuall position of the particular new we
		* enqueue this information here. Parsing and validy checks are done
		* by the function self */


	   parse_ns2_new_wireless_position(scenario, data);


	   /* now we save event relevant information like the reception
		* of a data packet et cetera */
	   parse_ns2_new_wireless_event(scenario, data);

	   account_simulation_end_time(scenario, data);

	   line_no++;

	}

	if (line)
		free(line);

	/* do traffic profile calculation for every
	 * node and the whole scenarion:
	 *
	 * 1. fseek() to the beginning of the file
	 * 2. while over the file
	 * 3. accumulate the bandwith consumption based
	 *    on a) node and b) protocol.
	 * 4. map these second samples to every node
	 */
	fseek(fp, 0L, SEEK_SET);
	calculate_traffic_profile(scenario, fp);

	fclose(fp);

	return scenario;
}




/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
