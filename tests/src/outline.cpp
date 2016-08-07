/*
* outline.c -- object outlining demo
* last modification: aug. 14, 2007
*
* Copyright (c) 2005-2007 David HENRY
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
* ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
* CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
* gcc -Wall -ansi -lGL -lGLU -lglut outline.c -o outline
*/

#include <GL/glut.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

static mouse_input_t mouse;
static glut_timer_t timer;
static Vector3d rot, eye;

typedef enum outlineMode_tag
{
	MODE_FIRST = 0,

	MODE_OUTLINE,
	MODE_OUTLINE_ONLY,
	MODE_SILHOUETTE,
	MODE_SILHOUETTE_ONLY,

	MODE_LAST

} outlineMode_t;
static outlineMode_t outlineMode = MODE_OUTLINE;

typedef enum objectType_tag
{
	OBJ_FIRST = MODE_LAST,

	OBJ_TEAPOT,
	OBJ_TEAPOT_CCW,
	OBJ_SPHERE,
	OBJ_CUBE,
	OBJ_TORUS,
	OBJ_QUAD,

	OBJ_LAST

} objectType_t;
static objectType_t objectType = OBJ_TEAPOT;

/*
* Application termination
*/
static void cleanup() { }

/*
* Application initialisation. Setup keyboard input,
* mouse input, timer, camera and OpenGL.
*/
static void init()
{
	/*
	* Application initialization
	*/

	/* Init mouse */
	mouse.buttons[GLUT_LEFT_BUTTON] = GLUT_UP;
	mouse.buttons[GLUT_MIDDLE_BUTTON] = GLUT_UP;
	mouse.buttons[GLUT_RIGHT_BUTTON] = GLUT_UP;
	mouse.x = 0;
	mouse.y = 0;

	/* Init camera input */
	rot.x = 0.0f;   eye.x = 0.0f;
	rot.y = 0.0f;   eye.y = 0.0f;
	rot.z = 0.0f;   eye.z = 4.5f;

	/*
	* OpenGL initialization
	*/
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
}

/*
* GLUT's reshape callback function.
* Update the viewport and the projection matrix.
*/
static void reshape(int w, int h)
{
	if (h == 0)
		h = 1;

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, w / (GLdouble)h, 0.1, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}

/*
* Draw the current object selected
*/
static void drawObject()
{
	switch (objectType)
	{
	case OBJ_TEAPOT:
		glFrontFace(GL_CW);
		glutSolidTeapot(1.0f);
		break;
	case OBJ_TEAPOT_CCW:
		glFrontFace(GL_CCW);
		glutSolidTeapot(1.0f);
		break;

	case OBJ_SPHERE:
		glFrontFace(GL_CCW);
		glutSolidSphere(1.0f, 50, 50);
		break;

	case OBJ_CUBE:
		glFrontFace(GL_CW);
		glutSolidCube(1.5f);
		break;

	case OBJ_TORUS:
		glFrontFace(GL_CCW);
		glutSolidTorus(0.3f, 1.0f, 10, 50);
		break;

	case OBJ_QUAD:
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);

		glVertex3f(-1.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);

		glNormal3f(0.0f, 0.0f, -1.0f);

		glVertex3f(-1.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glEnd();
		break;

	default:
		break;
	};
}

/*
* GLUT's display callback function.
* Render the main OpenGL scene.
*/
static void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	/* Camera rotation */
	glTranslated(-eye.x, -eye.y, -eye.z);
	glRotated(rot.x, 1.0f, 0.0f, 0.0f);
	glRotated(rot.y, 0.0f, 1.0f, 0.0f);
	glRotated(rot.z, 0.0f, 0.0f, 1.0f);

	/* Draw scene */
	switch (outlineMode)
	{
	case MODE_OUTLINE:
	{
		glPushAttrib(GL_POLYGON_BIT);
		glEnable(GL_CULL_FACE);

		/*
		* Draw front-facing polygons as filled
		*/

		glPolygonMode(GL_FRONT, GL_FILL);
		glCullFace(GL_BACK);

		/* Draw solid object */
		glColor3f(1.0f, 1.0f, 1.0f);
		drawObject();

		/*
		* Draw back-facing polygons as red lines
		*/

		/* Disable lighting for outlining */
		glPushAttrib(GL_LIGHTING_BIT | GL_LINE_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_LIGHTING);

		glPolygonMode(GL_BACK, GL_LINE);
		glCullFace(GL_FRONT);

		glDepthFunc(GL_LEQUAL);
		glLineWidth(5.0f);

		/* Draw wire object */
		glColor3f(1.0f, 0.0f, 0.0f);
		drawObject();

		/* GL_LIGHTING_BIT | GL_LINE_BIT | GL_DEPTH_BUFFER_BIT */
		glPopAttrib();

		/* GL_POLYGON_BIT */
		glPopAttrib();
		break;
	}

	case MODE_OUTLINE_ONLY:
	{
		glPushAttrib(GL_POLYGON_BIT);
		glEnable(GL_CULL_FACE);

		/*
		* Draw front-facing polygons as filled
		*/

		/* Disable color buffer */
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		glPolygonMode(GL_FRONT, GL_FILL);
		glCullFace(GL_BACK);

		/* Draw solid object */
		glColor3f(1.0f, 1.0f, 1.0f);
		drawObject();

		/*
		* Draw back-facing polygons as red lines
		*/

		/* Enable color buffer */
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		/* Disable lighting for outlining */
		glPushAttrib(GL_LIGHTING_BIT | GL_LINE_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_LIGHTING);

		glPolygonMode(GL_BACK, GL_LINE);
		glCullFace(GL_FRONT);

		glDepthFunc(GL_LEQUAL);
		glLineWidth(5.0f);

		/* Draw wire object */
		glColor3f(1.0f, 0.0f, 0.0f);
		drawObject();

		/* GL_LIGHTING_BIT | GL_LINE_BIT | GL_DEPTH_BUFFER_BIT */
		glPopAttrib();

		/* GL_POLYGON_BIT */
		glPopAttrib();
		break;
	}

	case MODE_SILHOUETTE:
	{
		glPushAttrib(GL_POLYGON_BIT);
		glEnable(GL_CULL_FACE);

		/*
		* Draw back-facing polygons as red lines
		*/

		/* Disable lighting for outlining */
		glPushAttrib(GL_LIGHTING_BIT | GL_LINE_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_LIGHTING);

		glPolygonMode(GL_BACK, GL_LINE);
		glCullFace(GL_FRONT);

		glDisable(GL_DEPTH_TEST);
		glLineWidth(5.0f);

		/* Draw wire object */
		glColor3f(1.0f, 0.0f, 0.0f);
		drawObject();

		/* GL_LIGHTING_BIT | GL_LINE_BIT | GL_DEPTH_BUFFER_BIT */
		glPopAttrib();

		/*
		* Draw front-facing polygons as filled
		*/

		glPolygonMode(GL_FRONT, GL_FILL);
		glCullFace(GL_BACK);

		/* Draw solid object */
		glColor3f(1.0f, 1.0f, 1.0f);
		drawObject();

		/* GL_POLYGON_BIT */
		glPopAttrib();
		break;
	}

	case MODE_SILHOUETTE_ONLY:
	{
		/* Clear stencil buffer */
		glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);

		/*
		* Draw front-facing polygons as filled
		*/

		/* Disable color and depth buffers */
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glDepthMask(GL_FALSE);

		/* Setup stencil buffer. Draw always in it */
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);
		glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

		/* Draw solid object to create a mask */
		glColor3f(1.0f, 1.0f, 1.0f);
		drawObject();

		/*
		* Draw back-facing polygons as red lines
		*/

		/* Enable color and depth buffers */
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDepthMask(GL_TRUE);

		/* Setup stencil buffer. We don't draw inside the mask */
		glStencilFunc(GL_NOTEQUAL, 1, 0xFFFFFFFF);

		/* Disable lighting for outlining */
		glPushAttrib(GL_LIGHTING_BIT | GL_LINE_BIT | GL_POLYGON_BIT);
		glDisable(GL_LIGHTING);

		glEnable(GL_CULL_FACE);

		glPolygonMode(GL_BACK, GL_LINE);
		glCullFace(GL_FRONT);

		glLineWidth(5.0f);

		/* Draw wire object */
		glColor3f(1.0f, 0.0f, 0.0f);
		drawObject();

		/* GL_LIGHTING_BIT | GL_LINE_BIT | GL_POLYGON_BIT */
		glPopAttrib();

		glDisable(GL_STENCIL_TEST);
		break;
	}

	default:
		break;
	}

	glutSwapBuffers();
}

/*
* GLUT's Key press callback function.
* Called when user press a key.
*/
static void keyPress(unsigned char key, int x, int y)
{
	/* Escape */
	if (key == 27)
		exit(0);

	glutPostRedisplay();
}

/*
* GLUT's mouse motion callback function.
* Called when the user move the mouse. Update the
* camera.
*/
static void mouseMotion(int x, int y)
{
	if (mouse.buttons[GLUT_RIGHT_BUTTON] == GLUT_DOWN)
	{
		/* Zoom */
		eye.z += (x - mouse.x) * 0.1;
	}
	else if (mouse.buttons[GLUT_LEFT_BUTTON] == GLUT_DOWN)
	{
		if (mouse.modifiers & GLUT_ACTIVE_SHIFT)
		{
			/* Translation */
			eye.x -= (x - mouse.x) * 0.02;
			eye.y += (y - mouse.y) * 0.02;
		}
		else
		{
			/* Rotation */
			rot.x += (y - mouse.y);
			rot.y += (x - mouse.x);
		}
	}

	mouse.x = x;
	mouse.y = y;

	glutPostRedisplay();
}

/*
* GLUT's mouse button callback function.
* Called when the user press a mouse button. Update mouse
* state and keyboard modifiers.
*/
static void mouseButton(int button, int state, int x, int y)
{
	/* Update key modifiers */
	mouse.modifiers = glutGetModifiers();

	/* Update mouse state */
	mouse.buttons[button] = state;
	mouse.x = x;
	mouse.y = y;
}

/*
* GLUT's menu callback function. Handle the menu.
* Select the outline mode to use and redraw the scene.
*/
static void menu(int item)
{
	if ((item > MODE_FIRST) && (item < MODE_LAST))
		outlineMode = (outlineMode_t)item;

	if ((item > OBJ_FIRST) && (item < OBJ_LAST))
		objectType = (objectType_t)item;

	glutPostRedisplay();
}

/*
* This is the main program.
*/
int outline_main(int argc, char** argv)
{
	int outlineModeId, objectTypeId;

	/* Initialize GLUT */
	glutInit(&argc, argv);

	/* Create an OpenGL window */
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Object outlining demo");

	/* Initialize application */
	atexit(cleanup);
	init();

	/* Create glut menu */
	outlineModeId = glutCreateMenu(menu);
	glutAddMenuEntry("Outline", MODE_OUTLINE);
	glutAddMenuEntry("Outline only", MODE_OUTLINE_ONLY);
	glutAddMenuEntry("Silhouette", MODE_SILHOUETTE);
	glutAddMenuEntry("Silhouette only", MODE_SILHOUETTE_ONLY);

	objectTypeId = glutCreateMenu(menu);
	glutAddMenuEntry("Teapot (Clockwise)", OBJ_TEAPOT);
	glutAddMenuEntry("Teapot (Counter-clockwise)", OBJ_TEAPOT_CCW);
	glutAddMenuEntry("Sphere", OBJ_SPHERE);
	glutAddMenuEntry("Cube", OBJ_CUBE);
	glutAddMenuEntry("Torus", OBJ_TORUS);
	glutAddMenuEntry("Quad", OBJ_QUAD);

	glutCreateMenu(menu);
	glutAddSubMenu("Outline mode", outlineModeId);
	glutAddSubMenu("Object", objectTypeId);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* Setup glut callback functions */
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPress);
	glutMotionFunc(mouseMotion);
	glutMouseFunc(mouseButton);

	/* Enter the main loop */
	glutMainLoop();

	return 0;
}
