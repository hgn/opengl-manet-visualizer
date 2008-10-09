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

float CAMERA_Pitch = 0.0f;
float CAMERA_Yaw = 0.0f;
float CAMERA_Move = 1.0f;

float xpos = 0, ypos = 0, zpos = 0, xrot = 0, yrot = 0, angle=0.0;

int screen_width, screen_height;

int frame,time,timebase=0;
char s[30];

void reshape(int w, int h)
{
	if (h == 0)
	{
		h = 1;
	}

	screen_width = w;
	screen_height = h;

	glViewport (0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective (45.0, (GLfloat)w/(GLfloat)h, 0.1, 100.0);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	glutPostRedisplay ();
}

static void render( float camera_yaw, float camera_pitch )
{
	draw_skybox(camera_yaw, camera_pitch);
	draw_ground();
	draw_terrain();
}

float lastx, lasty;

void mouseMovement(int x, int y)
{
	int diffx=x-lastx; //check the difference between the current x and the last x position
	int diffy=y-lasty; //check the difference between the current y and the last y position
	lastx=x; //set lastx to the current x position
	lasty=y; //set lasty to the current y position
	xrot += (float) diffy; //set the xrot to xrot with the addition of the difference in the y position
	yrot += (float) diffx;  //set the xrot to yrot with the addition of the difference in the x position
}

static void drawText(const char * message)
{
	/* raster pos sets the current raster position
	 * mapped via the modelview and projection matrices
	 */
	glColor4ub(0, 0, 0, 255);
	glRasterPos2f((GLfloat)7, (GLfloat)17);

	/*
	 * write using bitmap and stroke chars
	 */
	while (*message) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *message);
		*message++;
		//glutStrokeCharacter(GLUT_STROKE_ROMAN,*message++);
	}
}

static void drawText2(const char * message)
{
	/* raster pos sets the current raster position
	 * mapped via the modelview and projection matrices
	 */
	glColor4ub(0, 0, 0, 255);
	glRasterPos2f((GLfloat)7, (GLfloat)45);

	/*
	 * write using bitmap and stroke chars
	 */
	while (*message) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *message);
		*message++;
		//glutStrokeCharacter(GLUT_STROKE_ROMAN,*message++);
	}
}

static void drawText3(const char * message)
{
	/* raster pos sets the current raster position
	 * mapped via the modelview and projection matrices
	 */
	glColor4ub(0, 0, 0, 255);
	glRasterPos2f((GLfloat)7, (GLfloat)31);

	/*
	 * write using bitmap and stroke chars
	 */
	while (*message) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *message);
		*message++;
		//glutStrokeCharacter(GLUT_STROKE_ROMAN,*message++);
	}
}


static void draw_menu(void) {

	glViewport(0,0,screen_width,screen_height);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, screen_width,screen_height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glBegin(GL_QUADS);
	glColor4ub(200, 200, 200, 100);
	glVertex2i( 5, 50); // Bottom Left
	glVertex2i( 200, 50); // Bottom Right
	glVertex2i( 200, 5); // Top Right
	glVertex2i( 5, 5);
	glEnd();

#define MESSAGE	"MANET Network Visualizer"

	char *msg = MESSAGE;
	char s_time_str[64];
	snprintf(s_time_str, sizeof(s_time_str), "Time: %.2lfs", simulator_time());

	drawText(msg);
	drawText2(s);
	drawText3(s_time_str);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

static void renderBitmapString( float x, float y, float z, char *string)
{
	char *c;

	glRasterPos3f(x, y, z);

	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
	}
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

	glRotatef(xrot,1.0,0.0,0.0);
	glRotatef(yrot,0.0,1.0,0.0);
	glTranslated(-xpos,-ypos,-zpos);

	render(yrot, xrot);

	glPushMatrix();
	glColor3f( 1.0f, 1.0f, 0.0f );
	glRotatef(90., 1., 0., 0.);
	glTranslatef(0.0f, 0.0f, .9f);	// move z units out from the screen.
	draw_nodes();
	glPopMatrix();

#if 0
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


	map_draw_nodes();


	GLUquadricObj *cyl;
	cyl = gluNewQuadric();


	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

	glColor4f( 1.0f, 1.0f, 0.0f, 1.0f);
	renderBitmapString(0.0f, 0.5f, 0.0f, "Node 1");



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

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	renderBitmapString(2.0f, 0.5f, 0.0f, "Node 2");

	glDisable(GL_DEPTH_TEST );
	glDisable(GL_LIGHT0 );
	glDisable(GL_LIGHTING );
	glDisable(GL_COLOR_MATERIAL );

	draw_menu();

     frame++;
     time=glutGet(GLUT_ELAPSED_TIME);
     if (time - timebase > 1000) {
         sprintf(s,"FPS: %4.2f",frame*1000.0/(time-timebase));
         timebase = time;
         frame = 0;
     }




	glutSwapBuffers();
	glutPostRedisplay();
}

void special( int key, int x, int y )
{
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
	switch( key )
	{
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
		case 'g':
		{
			static int wireframe_enable = 0;

			wireframe_enable = !wireframe_enable;

			if( wireframe_enable )
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
