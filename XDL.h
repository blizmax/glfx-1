enum { XDL_BUTTON_LEFT, XDL_BUTTON_MIDDLE, XDL_BUTTON_RIGHT, XDL_BUTTON_COUNT };

typedef struct {

	// Window properties
	char *title;
	int width;
	int height;
	int frameRate;

	// Basic callbacks
	void (*init)(void);
	void (*update)(unsigned int timestamp);
	void (*redraw)(unsigned int timestamp);
	void (*exit)(void);

	// Mouse events
	void (*mouseMotion)(unsigned int timestamp, int x, int y, int dx, int dy);
	void (*mouseButtonDown[XDL_BUTTON_COUNT])(unsigned int timestamp, int x, int y);
	void (*mouseButtonUp[XDL_BUTTON_COUNT])(unsigned int timestamp, int x, int y);

} XDL_Options;

int XDL_Main(const XDL_Options *opts);
