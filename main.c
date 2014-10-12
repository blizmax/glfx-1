#include "XDL.h"
#include "glfx.h"

#include <math.h>

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

// Radius of colour picker
#define R0 160.0

// Radius of hue wheel
#define R1 200.0

extern char _binary_frag_glsl_start;

int grabbed = 0;
int grabX, grabY;
float grabAngle;

// Distance of given point from centre of window
float centreDist(int x, int y) {
	float xx = x - 0.5 * WINDOW_WIDTH;
	float yy = y - 0.5 * WINDOW_HEIGHT;
	return hypot(xx, yy);
}

// Angle of given point about centre of window
float centreAngle(int x, int y) {
	float xx = x - 0.5 * WINDOW_WIDTH;
	float yy = y - 0.5 * WINDOW_HEIGHT;
	return atan2(yy, xx);
}

GLint hueLoc;
GLfloat hue;

void opt_init(void) {
	glfxSetup(&_binary_frag_glsl_start);
	glUniform2f(glfxGetUniformLocation("windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);
	hueLoc = glfxGetUniformLocation("hue");
	glUniform1f(hueLoc, 0);
}

void opt_redraw(unsigned int timestamp) {
	glfxRedraw();
}

void opt_left_button_pressed(unsigned int timestamp, int x, int y) {
	float r = centreDist(x, y);
	if (r >= R0 && r < R1) {
		grabbed = 1;
		grabX = x;
		grabY = y;
		grabAngle = centreAngle(x, y);
	}
}

void opt_left_button_released(unsigned int timestamp, int x, int y) {
	if (grabbed) {
		grabbed = 0;
		hue += centreAngle(x, y) - grabAngle;
	}
}

void opt_cursor_moved(unsigned int timestamp, int x, int y, int dx, int dy) {
	if (grabbed) {
		glUniform1f(hueLoc, hue + centreAngle(x, y) - grabAngle);
	}
}

int main(int argc, char *argv) {
	XDL_Options opts = { "Demo", WINDOW_WIDTH, WINDOW_HEIGHT, -1 };

	opts.init   = opt_init;
	opts.redraw = opt_redraw;

	opts.mouseMotion                      = opt_cursor_moved;
	opts.mouseButtonDown[XDL_BUTTON_LEFT] = opt_left_button_pressed;
	opts.mouseButtonUp[XDL_BUTTON_LEFT]   = opt_left_button_released;

	return XDL_Main(&opts);
}
