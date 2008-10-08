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
#include <locale.h>

struct scenario *scenario;

int main(int ac, char **av)
{
	struct list_head *a_ev_l;

	setlocale(LC_ALL, "C");

	scenario = parse_offline_scenario(TRACE_FILE_NS2, "/usr/share/manet-visualizer/traces/ns2-new-wireless.tr");
	print_nodes_info(scenario);

	a_ev_l = init_active_event_list();
	setup_simulator_ref_time();

	glutInit(&ac, av);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Manet Visualizer");

	/* main gl handler */
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutSpecialFunc(special);

	/* user input handler */
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouseMovement);

	/* initalize mv specific routines */
	init_skybox();
	init_terrain();
	init_nodes();
	init_ground();

	while (23) {

		double s_time;
		struct event *next_event = NULL;


		s_time = simulator_time();
		next_event = peek_next_event();

		while (next_event && s_time >= next_event->time) {

			/* remove event from the global
			 * event queue cause the event
			 * is now processed in the active event list (a_ev_l)
			 */
			//list_del(&next_event->list);

			/* process all event in queue */
			//activate_event(next_event, a_ev_l);
			//next_event = peek_next_event();
		}

		/* display all event that are active now
		 * debug_display_nodes is also responsible to
		 * remove outdatet elements
		 */
		//debug_display_events(a_ev_l);

		/* and display the positions of the nodes
		 * at time s_time
		 */
		//debug_display_nodes_coordinates_at_time(scenario, s_time);



		glutMainLoop();
	}

	return EXIT_SUCCESS;
}



/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
