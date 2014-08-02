typedef struct {
	char *title;
	int width;
	int height;
	int frameRate;
	void (*init)(void);
	void (*update)(unsigned int timestamp);
	void (*redraw)(unsigned int timestamp);
	void (*exit)(void);
} XDL_Options;

int XDL_Main(const XDL_Options *opts);
