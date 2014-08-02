#include <SDL2/SDL.h>

#include "XDL.h"

enum { XDL_UPDATE = SDL_USEREVENT, XDL_REDRAW };

Uint32 XDL_TimerCallback_PushEvent(Uint32 interval, void *param) {
	SDL_PushEvent(param);
        return 0;
}

void XDL_EventLoop(SDL_Window *window, const XDL_Options *opts) {
	static SDL_Event updateEvent = { XDL_UPDATE };
	static SDL_Event redrawEvent = { XDL_REDRAW };
	SDL_Event event;
	int frameDelay = (opts->frameRate == -1) ? 0 : 
	                 (opts->frameRate > 0) ? 1000 / opts->frameRate : 
                         -1;

	SDL_PushEvent(&updateEvent);

	while (SDL_WaitEvent(&event)) switch (event.type) {
	case XDL_UPDATE:
		if (frameDelay == 0)
			SDL_PushEvent(&updateEvent);
		else if (frameDelay > 0)
			SDL_AddTimer(frameDelay, XDL_TimerCallback_PushEvent, &updateEvent);
		if (opts->update)
			opts->update(event.common.timestamp);
	case XDL_REDRAW:
		if (opts->redraw)
			opts->redraw(event.common.timestamp);
		SDL_GL_SwapWindow(window);
		break;
	case SDL_KEYUP:
		if (event.key.keysym.sym != SDLK_ESCAPE) break;
	case SDL_QUIT:
		return;
	}
}

SDL_Window *XDL_CreateWindow(const char *title, int width, int height) {
	int x = SDL_WINDOWPOS_UNDEFINED;
	int y = SDL_WINDOWPOS_UNDEFINED;
	Uint32 flags = SDL_WINDOW_OPENGL;
        return SDL_CreateWindow(title, x, y, width, height, flags);
}

int XDL_Main(const XDL_Options *opts) {
	SDL_Window *window;
	SDL_GLContext glContext;

	// Init SDL
	SDL_Init(SDL_INIT_TIMER|SDL_INIT_VIDEO);

	// Create window
        window = XDL_CreateWindow(opts->title, opts->width, opts->height); 

	// Create graphics context
	glContext = SDL_GL_CreateContext(window);

	// Dispatch init event
	if (opts->init)
		opts->init();

	// Execute main loop
	XDL_EventLoop(window, opts);

	// Dispatch exit event
	if (opts->exit)
		opts->exit();

	// Delete graphics context
	SDL_GL_DeleteContext(glContext);

	// Destroy window
	SDL_DestroyWindow(window);

	// Quit SDL
	SDL_Quit();

	return 0;
}
