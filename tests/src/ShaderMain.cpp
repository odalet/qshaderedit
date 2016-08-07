/* -*- c++ -*- */
/////////////////////////////////////////////////////////////////////////////
//
// Main.cpp  -- Copyright (c) 2006-2007 David Henry
// last modification: july 13, 2007
//
// This code is licenced under the MIT license.
//
// This software is provided "as is" without express or implied
// warranties. You may freely copy and compile this source into
// applications you distribute provided that the copyright text
// below is included in the resulting source code.
//
// Shader module test application.
//
/////////////////////////////////////////////////////////////////////////////

#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <cstdlib>

#include "Shader.h"
#include "common.h"

using std::cout;
using std::cerr;
using std::endl;

static mouse_input_t mouse;
static glut_timer_t timer;
static Vector3d rot, eye;

/* shader type */
typedef enum ShaderType_tag
{
	toon = 0,
	gooch,
	aachecker,
	brick,
	adaptanalytic,
	lattice,
	light,
	num_shaders

} ShaderType_t;
static ShaderType_t current_shader = toon;

bool enable_shader = true;
static ShaderProgram *shader[num_shaders];

int lightType = 1;

static int modifiers;

// --------------------------------------------------------------------------
// shutdownApp
//
// Application termination.
// --------------------------------------------------------------------------

static void shutdownApp()
{
	//ShaderManager::kill ();
}


// --------------------------------------------------------------------------
// init
//
// Application initialisation. Setup keyboard input, mouse input,
// timer, camera and OpenGL.
// --------------------------------------------------------------------------

static void init()
{
	// Initialize mouse
	mouse.buttons[GLUT_LEFT_BUTTON] = GLUT_UP;
	mouse.buttons[GLUT_MIDDLE_BUTTON] = GLUT_UP;
	mouse.buttons[GLUT_RIGHT_BUTTON] = GLUT_UP;
	mouse.x = 0;
	mouse.y = 0;

	// Initialize timer
	timer.curent_time = 0;
	timer.last_time = 0;

	// Initialize camera input
	rot.x = 0.0f;   eye.x = 0.0f;
	rot.y = 0.0f;   eye.y = 0.0f;
	rot.z = 0.0f;   eye.z = 7.0f;

	// Initialize OpenGL
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);

	// Initialize GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		// Problem: glewInit failed, something is seriously wrong.
		cerr << "Error: " << glewGetErrorString(err) << endl;
	}

	// Initialize shaders
	VertexShader vertToon("shaders/toon.vert");
	FragmentShader fragToon("shaders/toon.frag");
	shader[toon] = new ShaderProgram("toon", vertToon, fragToon);

	VertexShader vertGooch("shaders/gooch.vert");
	FragmentShader fragGooch("shaders/gooch.frag");
	shader[gooch] = new ShaderProgram("gooch", vertGooch, fragGooch);

	VertexShader vertAAChecker("shaders/aachecker.vert");
	FragmentShader fragAAChecker("shaders/aachecker.frag");
	shader[aachecker] = new ShaderProgram("aachecker", vertAAChecker, fragAAChecker);

	VertexShader vertBrick("shaders/brick.vert");
	FragmentShader fragBrick("shaders/brick.frag");
	shader[brick] = new ShaderProgram("brick", vertBrick, fragBrick);

	VertexShader vertAdaptanalytic("shaders/adaptanalytic.vert");
	FragmentShader fragAdaptanalytic("shaders/adaptanalytic.frag");
	shader[adaptanalytic] = new ShaderProgram("adaptanalytic", vertAdaptanalytic,
		fragAdaptanalytic);

	VertexShader vertLattice("shaders/lattice.vert");
	FragmentShader fragLattice("shaders/lattice.frag");
	shader[lattice] = new ShaderProgram("lattice", vertLattice, fragLattice);

	VertexShader vertLight("shaders/light.vert");
	FragmentShader fragLight("shaders/light.frag");
	shader[light] = new ShaderProgram("light", vertLight, fragLight);
}


// --------------------------------------------------------------------------
// reshape
//
// OpenGL window resizing.  Update the viewport and the projection matrix.
// --------------------------------------------------------------------------

static void reshape(int w, int h)
{
	if (h == 0) h = 1;

	glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, w / static_cast<GLfloat>(h), 0.1f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}


// --------------------------------------------------------------------------
// updateTimer
//
// Update the timer.
// --------------------------------------------------------------------------

static void updateTimer(struct glut_timer_t* t)
{
	t->last_time = t->curent_time;
	t->curent_time = static_cast<double> (glutGet(GLUT_ELAPSED_TIME)) * 0.001f;
}


// --------------------------------------------------------------------------
// drawToonShader
//
// Render the 'toon' scene.
// --------------------------------------------------------------------------

static void drawToonShader()
{
	// Setup shader
	shader[toon]->use();

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	// Draw front-facing polygons as filled using toon shader
	glPolygonMode(GL_FRONT, GL_FILL);
	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glutSolidTeapot(1.0f);

	shader[toon]->unuse();

	// Draw back-facing polygons as black lines using standard OpenGL
	glLineWidth(5.0f);
	glPolygonMode(GL_BACK, GL_LINE);
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_FRONT);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidTeapot(1.0f);

	glLineWidth(1.0f);
	glDepthFunc(GL_LESS);
	glPolygonMode(GL_BACK, GL_FILL);

	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}


// --------------------------------------------------------------------------
// drawGoochShader
//
// Render the 'gooch' scene.
// --------------------------------------------------------------------------

static void drawGoochShader()
{
	// Setup shader
	shader[gooch]->use();

	GLint loc;
	loc = glGetUniformLocation(shader[gooch]->handle(), "SurfaceColor");
	glUniform3f(loc, 0.75f, 0.75f, 0.75f);

	loc = glGetUniformLocation(shader[gooch]->handle(), "WarmColor");
	glUniform3f(loc, 0.6f, 0.6f, 0.0f);

	loc = glGetUniformLocation(shader[gooch]->handle(), "CoolColor");
	glUniform3f(loc, 0.0f, 0.0f, 0.6f);

	loc = glGetUniformLocation(shader[gooch]->handle(), "DiffuseWarm");
	glUniform1f(loc, 0.45f);

	loc = glGetUniformLocation(shader[gooch]->handle(), "DiffuseCool");
	glUniform1f(loc, 0.45f);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	// Draw front-facing polygons as filled using gooch shader
	glPolygonMode(GL_FRONT, GL_FILL);
	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glutSolidTeapot(1.0f);

	shader[gooch]->unuse();

	// Draw back-facing polygons as black lines using standard OpenGL
	glLineWidth(5.0f);
	glPolygonMode(GL_BACK, GL_LINE);
	glDepthFunc(GL_LEQUAL);
	glCullFace(GL_FRONT);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidTeapot(1.0f);

	glLineWidth(1.0f);
	glDepthFunc(GL_LESS);
	glPolygonMode(GL_BACK, GL_FILL);

	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
}


// --------------------------------------------------------------------------
// drawAACheckerShader
//
// Render the 'antialiased checker' scene.
// --------------------------------------------------------------------------

static void drawAACheckerShader()
{
	// Setup shader
	shader[aachecker]->use();

	GLint loc;
	loc = glGetUniformLocation(shader[aachecker]->handle(), "Color1");
	glUniform3f(loc, 1.0f, 1.0f, 1.0f);

	loc = glGetUniformLocation(shader[aachecker]->handle(), "Color2");
	glUniform3f(loc, 1.0f, 0.5f, 0.0f);

	loc = glGetUniformLocation(shader[aachecker]->handle(), "AvgColor");
	glUniform3f(loc, 0.5f, 0.5f, 0.5f);

	loc = glGetUniformLocation(shader[aachecker]->handle(), "Frequency");
	glUniform1f(loc, 2.0f);

	glutSolidTeapot(1.0f);

	shader[aachecker]->unuse();
}


// --------------------------------------------------------------------------
// drawBrickShader
//
// Render the 'brick' scene.
// --------------------------------------------------------------------------

static void drawBrickShader()
{
	// Setup shader
	shader[brick]->use();

	GLint loc;
	loc = glGetUniformLocation(shader[brick]->handle(), "BrickColor");
	glUniform3f(loc, 1.0f, 0.3f, 0.2f);

	loc = glGetUniformLocation(shader[brick]->handle(), "MortarColor");
	glUniform3f(loc, 0.85f, 0.86f, 0.84f);

	loc = glGetUniformLocation(shader[brick]->handle(), "BrickSize");
	glUniform2f(loc, 0.30f, 0.15f);

	loc = glGetUniformLocation(shader[brick]->handle(), "BrickPct");
	glUniform2f(loc, 0.90f, 0.85f);

	glutSolidTeapot(1.0f);

	shader[brick]->unuse();
}


// --------------------------------------------------------------------------
// drawAdaptanalyticShader
//
// Render the 'adaptic analytic' scene.
// --------------------------------------------------------------------------

static void drawAdaptanalyticShader()
{
	// Setup shader
	shader[adaptanalytic]->use();

	GLint loc;
	loc = glGetUniformLocation(shader[adaptanalytic]->handle(), "Frequency");
	glUniform1f(loc, 6.0f);

	glutSolidTeapot(1.0f);

	shader[adaptanalytic]->unuse();
}


// --------------------------------------------------------------------------
// drawLatticeShader
//
// Render the 'lattice' scene.
// --------------------------------------------------------------------------

static void drawLatticeShader()
{
	// Setup shader
	shader[lattice]->use();

	GLint loc;
	loc = glGetUniformLocation(shader[lattice]->handle(), "Scale");
	glUniform2f(loc, 10.0f, 10.0f);

	loc = glGetUniformLocation(shader[lattice]->handle(), "Threshold");
	glUniform2f(loc, 0.13f, 0.13f);

	loc = glGetUniformLocation(shader[lattice]->handle(), "SurfaceColor");
	glUniform3f(loc, 0.9f, 0.7f, 0.25f);

	glutSolidTeapot(1.0f);

	shader[lattice]->unuse();
}


// --------------------------------------------------------------------------
// drawLightShader
//
// Render the 'light' scene.
// --------------------------------------------------------------------------

static void drawLightShader()
{
	// Setup shader
	shader[light]->use();

	GLint loc;
	loc = glGetUniformLocation(shader[light]->handle(), "lightType");
	glUniform1i(loc, lightType);

	glutSolidTeapot(1.0f);

	shader[light]->unuse();
}


// --------------------------------------------------------------------------
// drawDefaultScene
//
// Render the default scene (when there is no shader enabled).
// --------------------------------------------------------------------------

static void drawDefaultScene()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);
	glutSolidTeapot(1.0f);
	glDisable(GL_LIGHTING);
}


// --------------------------------------------------------------------------
// display
//
// Render the main OpenGL scene.
// --------------------------------------------------------------------------

static void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

#if 0
	// Setup camera
	glTranslatef(0.0f, 0.0f, -5.0f);

	// Setup light position
	glPushMatrix();
	static GLfloat angle = 0.0f;
	static GLfloat y_axis = 2.0f;
	static int angle_dir = 1;
	static int y_dir = 1;

	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 3.0f);
	glTranslatef(0.0f, y_axis, 0.0f);

	if (1)
	{
		// Draw light position
		glPointSize(5.0f);
		glBegin(GL_POINTS);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glEnd();
	}

	angle += 0.1f * angle_dir;
	if ((angle > 90.0f) || (angle < -90.0f))
		angle_dir *= -1;

	y_axis += 0.01f * y_dir;
	if ((y_axis > 3.0f) || (y_axis < -3.0f))
		y_dir *= -1;

	GLfloat lightPos[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0f);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 15.0f);
	GLfloat lightDir[4] = { 0.0f, 0.0f, -1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDir);

	GLfloat lightColor[4] = { 0.7f, 0.3f, 0.3f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glPopMatrix();
#else
	GLfloat lightPos[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	if (lightType == 0) lightPos[3] = 0.0f;

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	if (lightType == 2)
	{
		glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0f);
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 7.0f);
		GLfloat lightDir[4] = { 0.0f, 0.0f, -1.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDir);
	}
	else
	{
		glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0.0f);
		glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0f);
	}

	GLfloat lightColor[4] = { 0.7f, 0.3f, 0.3f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);

	// Camera rotation
	glTranslated(-eye.x, -eye.y, -eye.z);
	glRotated(rot.x, 1.0f, 0.0f, 0.0f);
	glRotated(rot.y, 0.0f, 1.0f, 0.0f);
	glRotated(rot.z, 0.0f, 0.0f, 1.0f);
#endif

	// Draw the scene
	if (enable_shader)
	{
		switch (current_shader)
		{
		case toon:
			drawToonShader();
			break;

		case gooch:
			drawGoochShader();
			break;

		case aachecker:
			drawAACheckerShader();
			break;

		case brick:
			drawBrickShader();
			break;

		case adaptanalytic:
			drawAdaptanalyticShader();
			break;

		case lattice:
			drawLatticeShader();
			break;

		case light:
			drawLightShader();
			break;

		default:
			drawDefaultScene();
			break;
		}
	}
	else drawDefaultScene();

	glutSwapBuffers();
}

// --------------------------------------------------------------------------
// keyPress
//
// Key press glut callback function. Called when user press a key.
// Handle ponctual actions.
// --------------------------------------------------------------------------

static void keyPress(unsigned char key, int x, int y)
{
	// escape
	if (key == 27)
		exit(0);

	// 'l': switch light type
	if (key == 'l' || key == 'L')
		lightType = (lightType + 1) % 3;

	// 's': toggle shader
	if (key == 's' || key == 'S')
		enable_shader = !enable_shader;

	glutPostRedisplay();
}


// --------------------------------------------------------------------------
// mouseMotion
//
// Mouse motion glut callback function. Called when the user move the
// mouse. Update the camera and redraw the scene.
// --------------------------------------------------------------------------

static void
mouseMotion(int x, int y)
{
	if (mouse.buttons[GLUT_MIDDLE_BUTTON] == GLUT_DOWN)
	{
		// Zoom
		eye.z += (x - mouse.x) * 0.1;
	}
	else if (mouse.buttons[GLUT_LEFT_BUTTON] == GLUT_DOWN)
	{
		if (modifiers & GLUT_ACTIVE_SHIFT)
		{
			// Translation
			eye.x -= (x - mouse.x) * 0.02;
			eye.y += (y - mouse.y) * 0.02;
		}
		else
		{
			// Rotation
			rot.x += (y - mouse.y);
			rot.y += (x - mouse.x);
		}
	}

	mouse.x = x;
	mouse.y = y;

	glutPostRedisplay();
}


// --------------------------------------------------------------------------
// mouseButton
//
// Mouse button press glut callback function. Called when the user
// press a mouse button. Update mouse state and keyboard modifiers.
// --------------------------------------------------------------------------

static void
mouseButton(int button, int state, int x, int y)
{
	// Update key modifiers
	modifiers = glutGetModifiers();

	// Update mouse state
	mouse.buttons[button] = state;
	mouse.x = x;
	mouse.y = y;
}


// --------------------------------------------------------------------------
// menu
//
// GLUT menu callback function. Handle the menu. Select the shader
// to use and redraw the scene.
// --------------------------------------------------------------------------

static void menu(int item)
{
	current_shader = static_cast<ShaderType_t> (item);
	glutPostRedisplay();
}


// --------------------------------------------------------------------------
// idleVisible
//
// Idle glut callback function. Continuously called. Perform background
// processing tasks.
// --------------------------------------------------------------------------

static void
idleVisible()
{
	// Update the timer
	updateTimer(&timer);
}

// --------------------------------------------------------------------------
// windowStatus
//
// Window status glut callback function. Called when the status of
// the window changes.
// --------------------------------------------------------------------------
static void windowStatus(int state)
{
	// Disable rendering and/or animation when the window is not visible
	glutIdleFunc(state != GLUT_HIDDEN && state != GLUT_FULLY_COVERED ? idleVisible : nullptr);
}


// --------------------------------------------------------------------------
// main
//
// Application main entry point.
// --------------------------------------------------------------------------
int shader_main(int argc, char** argv)
{
	// Initialize glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Shader module");

	// Initialize application
	atexit(shutdownApp);
	init();

	// Create glut menu
	glutCreateMenu(menu);
	glutAddMenuEntry("Toon shader", toon);
	glutAddMenuEntry("Gooch matte", gooch);
	glutAddMenuEntry("Antialiased Checker", aachecker);
	glutAddMenuEntry("Procedural bricks", brick);
	glutAddMenuEntry("Adaptive analytic antialiasing", adaptanalytic);
	glutAddMenuEntry("Lattice", lattice);
	glutAddMenuEntry("Per pixel lighting", light);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	// Setup glut callback functions
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyPress);
	glutMotionFunc(mouseMotion);
	glutMouseFunc(mouseButton);
	glutWindowStatusFunc(windowStatus);
	glutIdleFunc(idleVisible);

	// Enter the main loop
	glutMainLoop();

	return 0;
}
