#pragma once

/* mouse */
struct mouse_input_t
{
	int buttons[3];
	int x, y;

	int modifiers;
};

/* timer */
struct glut_timer_t
{
	double curent_time;
	double last_time;
};

/* camera */
struct Vector3d
{
	float x, y, z;
};