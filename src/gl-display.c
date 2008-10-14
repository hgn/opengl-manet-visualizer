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

extern int simulation_paused;

float CAMERA_Pitch = 0.0f;
float CAMERA_Yaw = 0.0f;
float CAMERA_Move = 1.0f;

float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle=0.0;

extern struct globals globals;

void reshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	globals.screen_width = w;
	globals.screen_height = h;

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 0.1, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity ();

	glutPostRedisplay ();
}

float lastx, lasty;

void mouseMovement(int x, int y)
{
	int diffx = x - lastx; //check the difference between the current x and the last x position
	int diffy = y - lasty; //check the difference between the current y and the last y position
	lastx = x; //set lastx to the current x position
	lasty = y; //set lasty to the current y position
	xrot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
	yrot += (float) diffx;  //set the xrot to yrot with the addition of the difference in the x position
}


void display(void)
{

	/* update the global time first */

	glEnable( GL_COLOR_MATERIAL );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
	glEnable( GL_DEPTH_TEST );

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glColor3f( 1.0f, 1.0f, 1.0f );

	glRotatef(xrot, 1.0, 0.0, 0.0);
	glRotatef(yrot, 0.0, 1.0, 0.0);
	glTranslated(-xpos, -ypos, -zpos);

	draw_skybox(yrot, xrot);
	draw_terrain();

	map_draw_nodes();

	visualize_packets();

#if 0
	glPushMatrix();
	glColor3f( 1.0f, 1.0f, 0.0f );
	glRotatef(90., 1., 0., 0.);
	glTranslatef(0.0f, 0.0f, .9f);	// move z units out from the screen.
	draw_nodes();
	glPopMatrix();

	glBegin(GL_QUADS);	// begin drawing a cube
	// Surface
	glNormal3f(0.0f, 0.0f, 0.0f);
	glColor3f( 0.0f, 0.0f, 0.0f );
	glVertex3f(-100.0f, -1.1f, -100.0f);
	glVertex3f(-100.0f, -1.1f, 100.0f);
	glVertex3f(100.0f, -1.1f, 100.0f);
	glVertex3f(100.0f, -1.1f, -100.0f);
	glEnd();		// done with the polygon.
#endif

	/* red lines - grid */
#if 0
	glBegin(GL_LINES);
	glLineWidth(1.0);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glColor3f( 1.0f, 0.0f, 0.0f );
	{
		double i;
	for(i = -100; i <= 100; i += 1) {
	glVertex3f(i, -0.9f, 100.0f);
	glVertex3f(i, -0.9f, -100.0f);
	}
	for(i = -100; i <= 100; i += 1) {
	glVertex3f(100.0, -0.9f, i);
	glVertex3f(-100.0, -0.9f, i);
	}
	}
	glEnd();		// done with the polygon.
#endif


	/* draw all nodes, this includes
	 *  o node illustration
	 *  o node infotext
	 */


	GLUquadricObj *cyl;
	cyl = gluNewQuadric();


	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if 0

	/* cylinder */
	glPushMatrix();
	glColor3f( 1.0f, 1.0f, 0.0f );
	glRotatef(90., 1., 0., 0.);
	glTranslatef(0.0f, 0.0f, .9f);	// move z units out from the screen.
	draw_nodes();


	glPopMatrix();
	/* disc */
	glPushMatrix();
	glColor4f( 1.0f, 1.0f, 0.0f,0.4f);
	glRotatef(90., 1., 0., 0.);
	glTranslatef(0.0f, 0.0f, 0.91f);	// move z units out from the screen.
	GLUquadricObj *quadratic;
	quadratic=gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
	gluQuadricTexture(quadratic, GL_TRUE);		// Create Texture Coords ( NEW )
	gluDisk(quadratic,0.2f,2.5f,32,32);

	glPopMatrix();




	/* cylinder */
	glPushMatrix();

	glColor3f( 0.0f, 0.0f, 1.0f );
	glTranslatef(2.0f, -1.0f, 0.0);	// move z units out from the screen.
	gluQuadricDrawStyle(cyl, GLU_LINE);
	draw_nodes();
	glPopMatrix();
	/* disc */
	glPushMatrix();
	glColor4f( 0.0f, 0.0f, 1.0f, 0.4f);
	glRotatef(90., 1., 0., 0.);
	glTranslatef(2.0f, 0.0f, .9f);	// move z units out from the screen.
	GLUquadricObj *quadratic1;
	quadratic1=gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
	gluQuadricNormals(quadratic1, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
	gluQuadricTexture(quadratic1, GL_TRUE);		// Create Texture Coords ( NEW )
	gluDisk(quadratic1,0.2f,2.5f,32,32);
	glPopMatrix();

#endif

	glDisable(GL_DEPTH_TEST );
	glDisable(GL_LIGHT0 );
	glDisable(GL_LIGHTING );
	glDisable(GL_COLOR_MATERIAL );

	draw_infobox();

	glutSwapBuffers();
	glutPostRedisplay();
}

void special( int key, int x, int y )
{
	(void) x;
	(void) y;

	switch( key )
	{
		// Deplacement de la camera
		case GLUT_KEY_UP:
		{
			CAMERA_Pitch -= CAMERA_Move;
			break;
		}

		case GLUT_KEY_DOWN:
		{
			CAMERA_Pitch += CAMERA_Move;
			break;
		}

		case GLUT_KEY_LEFT:
		{
			CAMERA_Yaw -= CAMERA_Move;
			break;
		}

		case GLUT_KEY_RIGHT:
		{
			CAMERA_Yaw += CAMERA_Move;
			break;
		}
	}
}

void keyboard(unsigned char key, int x, int y)
{
	(void) x;
	(void) y;

	switch( key ) {
		case 27 :
			exit(0);
			break;
		case 'q':
			xrot += 1;
			if (xrot >360) xrot -= 360;
			break;
		case 'z':
			xrot -= 1;
			if (xrot < -360) xrot += 360;
			break;
		case 'w':
			{
				float xrotrad, yrotrad;
				yrotrad = (yrot / 180 * 3.141592654f);
				xrotrad = (xrot / 180 * 3.141592654f);
				xpos += (float) sin(yrotrad) ;
				zpos -= (float) cos(yrotrad) ;
				ypos -= (float) sin(xrotrad) ;
				if (ypos < 0)
					ypos = 0.0;
			}
			break;
		case 's':
			{
				float xrotrad, yrotrad;
				yrotrad = (yrot / 180 * 3.141592654f);
				xrotrad = (xrot / 180 * 3.141592654f);
				xpos -= (float) sin(yrotrad);
				zpos += (float) cos(yrotrad);
				ypos += (float) sin(xrotrad);
				if (ypos < 0)
					ypos = 0.0;
			}
			break;
		case 'd':
			{
				float yrotrad;
				yrotrad = (yrot / 180 * 3.141592654f);
				xpos += (float) cos(yrotrad) * 0.2;
				zpos += (float) sin(yrotrad) * 0.2;
			}
			break;
		case 'a':
			{
				float yrotrad;
				yrotrad = (yrot / 180 * 3.141592654f);
				xpos -= (float) cos(yrotrad) * 0.2;
				zpos -= (float) sin(yrotrad) * 0.2;
			}
			break;
		case 'p':
			if (simulation_paused) {
				/* we are restarting our simulation, so
				 * reinit the simulation reference time */
				setup_simulator_ref_time();
			}
			simulation_paused = !simulation_paused;
			break;
		case 'g':
			{
				static int wireframe_enable = 0;

				wireframe_enable = !wireframe_enable;

				if (wireframe_enable)
				{
					glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
				}
				else
				{
					glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
				}
			}
	}
}



/* vim: set tw=78 ts=4 sw=4 sts=4 ff=unix noet: */
