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


#define	EVENT_TYPE 0
#define	TIME 2
#define	NODE_ID 4
#define	POS_X 6
#define	POS_Y 8

#define	PROTO_L 10

int parse_ns2_new_wireless_position(struct scenario *s, char *data[])
{
	double mtime;
	int32_t x, y, nodeid;
	struct node *node;

	assert(s);

	/* some sanity checks first */
	if (!data[TIME] || !data[NODE_ID] || !data[POS_X] || !data[POS_Y])
		return 0;

	/*
	fprintf(stderr, "time %s id %s x %s y %s\n",
			data[TIME], data[NODE_ID], data[POS_X], data[POS_Y]);
			*/

	mtime   = strtod(data[TIME], NULL);
	nodeid = atoi(data[NODE_ID]);
	x      = atoi(data[POS_X]);
	y      = atoi(data[POS_Y]);

	node = get_node_by_id(s, nodeid);
	if (!node) {
		fprintf(stderr, "new node %d detected\n", nodeid);
		node = alloc_node(nodeid);
		add_node_to_scenario(s, node);
	}

	/* ok, node is a valid point, add a new list entry */
	add_position_data_to_node(node, mtime, x, y);

	return 0;
}

static enum event_type categorize_event(char *data[])
{
	if (!strcmp(data[PROTO_L], "agt")) {
		return ET_CBR_IN;
	} else {
		return ET_UNKNOWN;
	}
}

static void entail_cbr_in(struct scenario *s, char *time)
{
}


/**
 * parse ns2 new wireless trace line and enqueue important events
 * to the event list within scenario
 */
void parse_ns2_new_wireless_event(struct scenario *s, char *data[])
{
	enum event_type et;

	assert(s);

	/* categorize event */
	et = categorize_event(data);

	switch (et) {
		case ET_CBR_IN:
			entail_cbr_in(s, data[TIME]);
			break;
		case ET_UNKNOWN: /* fall through */
			fprintf(stderr, "Parser error, data within the trace file seems to be corrupt\n");
			break;
		default:
			fprintf(stderr, "Programmed error in switch/case statement\n");
			exit(EXIT_FAILURE);
	}
}


static struct scenario *parse_ns2_new_wireless_scenario(const char *file)
{
	struct scenario *scenario;
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	scenario = alloc_scenario();


	fp = fopen(file, "r");
	if (fp == NULL) {
		fprintf(stderr, "Can't open trace file %s:%s\n",
				file, strerror(errno));
		exit(EXIT_FAILURE);
	}

	int first_round = 1;

	while ((read = getline(&line, &len, fp)) != -1) {

		int iter;
		char *data[64];

		/* null out point array  */
		for (iter = 0; iter < 64; iter++)
			data[iter] = NULL;

		/* format description:
		 * http://www.isi.edu/nsnam/ns/doc/node186.html
		 */

		iter = 0;

		data[iter++] = strtok(line, " ");

		while ((data[iter++] = strtok(NULL, " ")))
			;

#if 0
		FIXME: active this later if the format is clearly parsed
		if (iter != 20) {
			fprintf(stderr, "line %s seems to be no valid ns2 trace line, ignoring ...\n");
			continue;
		}
#endif

		/* ok, this is a valid line, because every line in the new trace file
		 * format contains the actuall position of the particular new we
		 * enqueue this information here. Parsing and validy checks are done
		 * by the function self */
		parse_ns2_new_wireless_position(scenario, data);

		/* now we save event relevant information like the reception
		 * of a data packet et cetera */
		parse_ns2_new_wireless_event(scenario, data);

	}


	if (line)
		free(line);

	fclose(fp);

	return scenario;

}

struct scenario *parse_offline_scenario(int format, const char *filename)
{
	switch (format) {
		case TRACE_FILE_FAKE:
			break;
		case TRACE_FILE_NS2:
			return parse_ns2_new_wireless_scenario(filename);
			break;
		default:
			die("Programmed error in switch/case statementn");
			break;
	}
}





/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
