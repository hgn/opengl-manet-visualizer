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

#include <getopt.h>

extern char *optarg;
static const char *optstr = "t:h?";

static const struct option lopts[] = {
	{ "tracefile", required_argument, NULL, 't' },

	{ "format", required_argument, NULL, 'f' },
	{ "help", no_argument, NULL, 'h' },
	{ NULL, no_argument, NULL, 0 }
};

static void display_usage(void)
{
	return;
}


struct opts *parse_cl_options(int ac, char **av)
{
	int opt = 0, inx = 0, ret;
	struct opts *opts;
	uint32_t mask;

	opts = xalloc(sizeof(struct opts));

	/* initial set ups - defaults */
    //opts->tracefile            = DEFAULT_POPULATION_SIZE;
	//opts->traceformat         = DEFAULT_MUTATION_MODE;

	opt = getopt_long(ac, av, optstr, lopts, &inx );

	while( opt != -1 ) {
		switch( opt ) {
			case 'h':
			case '?':
				display_usage();
				exit(EXIT_SUCCESS);
				break;
			default:
				free(opts);
				fputs("Command-line option not supported", stderr);
				display_usage();
				return NULL;
				break;
		}

		opt = getopt_long(ac, av, optstr, lopts, &inx );
	}

	return opts;
}


/* vi: set tw=78 sw=4 ts=4 sts=4 ff=unix noet: */
