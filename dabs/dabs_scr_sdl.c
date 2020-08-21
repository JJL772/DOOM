/*

NOTES:

- The backbuffer and frontbuffer are both in an 8-bit index format, as that's what DOOM uses 
- The frontbuffer isn't as much of a frontbuffer as it is a translation area for the backbuffer 
- Currently, there is an extra thread created for the renderer to use to render to the screen

TODO: Need to find a way to implement vertical syncing, because the render thread kinda just copies shit out of the main thread
to render, so we will probably end up with some pretty terrible screen tearing

*/
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_thread.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#endif 

// Only one window supported right now
typedef struct dabs_window_s
{
	SDL_Window* hwnd;
	SDL_Renderer* hrenderer;
	SDL_Surface* hsurf;
	const char* title;
	int w, h, depth;
} dabs_window_t;

dabs_window_t g_window;
Uint64 g_main_thread;
SDL_Thread* g_render_thread;
char* g_backbuffer;
char* g_frontbuffer;

// forward decls 
 _Noreturn static void SCR_RenderThreadFunc(void*);

void SCR_Init() {};

/**
 * Creates a window with the width, height, title and depth 
 */
void* SCR_CreateWindow(int w, int h, const char* title, int depth)
{

	/* Create the window */
	g_window.hwnd = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
	g_window.w = w;
	g_window.h = h;
	g_window.title = title;
	g_window.depth = depth;
	if(!g_window.hwnd) return NULL;
	g_window.hsurf = SDL_GetWindowSurface(g_window.hwnd);

	/* Allocate the back and front buffers */
	g_backbuffer = malloc(w * h);
	g_frontbuffer = malloc(w * h);
	memset(g_backbuffer, 0, w*h);
	memset(g_frontbuffer, 0, w*h);

	/* Create the software renderer used to actually render to the screen */
	g_window.hrenderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(g_window.hwnd));
	if(!g_window.hrenderer) return NULL;

#ifndef _WIN32
	g_main_thread = syscall(SYS_gettid);
#else
	#error NOT IMPLEMENTED
#endif 
	/* Create the render thread */
	g_render_thread = SDL_CreateThread(SCR_RenderThreadFunc, "RenderThread", NULL);

	return &g_window;
}

/**
 * Returns a pointer to the framebuffer of the window
 */ 
void* SCR_GetFramebuffer(void* wnd)
{
	return g_backbuffer;
}

void SCR_DestroyWindow(void* wnd)
{
	SDL_DestroyRenderer(g_window.hrenderer);
	SDL_DestroyWindow(g_window.hwnd);
	g_window.hwnd = g_window.hrenderer = NULL;
}

static void SCR_RenderThreadFunc(void* param)
{
	printf("Render thread initialized...\n");
	while(1)
	{
		SDL_Delay(5);
		
		/* Copy the renderbuffer to the work-area, (aka g_frontbuffer) */
		/* Need to make this thread-safe and synchronize it with each frame */
		memcpy(g_frontbuffer, g_backbuffer, g_window.w * g_window.h);

		SDL_LockSurface(g_window.hsurf);

		/* Convert the indexed buffer to the proper format */
		switch(g_window.hsurf->format->BitsPerPixel)
		{
			case 8:
			{

			}
			case 16:
			{

			}
			case 24:
			{

			}
			case 32:
			{

			}
			default: break;
		}

		SDL_UnlockSurface(g_window.hsurf);

	}
}
