#include "XDL.h"
#include "glfx.h"

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

extern char _binary_frag_glsl_start;

GLint tLoc;

void opt_init(void) {
	glfxSetup(&_binary_frag_glsl_start);
	tLoc = glfxGetUniformLocation("t");
	glUniform2f(glfxGetUniformLocation("windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);
}

void opt_update(unsigned int timestamp) {
}

void opt_redraw(unsigned int timestamp) {
	glUniform1f(tLoc, timestamp * 1e-3);
	glfxRedraw();
}

int main(int argc, char *argv) {
	XDL_Options opts;

	opts.title     = "Demo";
	opts.width     = WINDOW_WIDTH;
	opts.height    = WINDOW_HEIGHT;
	opts.frameRate = -1;
	opts.init      = opt_init;
	opts.update    = opt_update;
	opts.redraw    = opt_redraw;
	opts.exit      = NULL;

	return XDL_Main(&opts);
}
