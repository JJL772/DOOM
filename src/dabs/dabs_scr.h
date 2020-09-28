/**
 *
 * dabs_window.h - Window definition
 *
 */
#pragma once

/* Initializes the video subsystem */
void SCR_Init();

/**
 * Creates a window with the width, height, title and depth 
 */
void* SCR_CreateWindow(int w, int h, const char* title, int depth);

/**
 * Returns a pointer to the framebuffer of the window
 */ 
void* SCR_GetFramebuffer(void* wnd);

void SCR_DestroyWindow(void* wnd);
