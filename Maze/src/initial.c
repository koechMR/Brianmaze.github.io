#include "../headers/maze.h"
#include "../../../../usr/include/SDL2/SDL_video.h"
#include "../../../../usr/include/SDL2/SDL_render.h"

/**
 * safe_close_sdl - closes sdl
 * @sdl: data
 * Return: nothing
 */
void safe_close_sdl(sdl_instance *sdl)
{
	if (sdl->window)
		SDL_DestroyWindow(sdl->window);
	if (sdl->renderer)
		SDL_DestroyRenderer(sdl->renderer);
	if (sdl->skybox)
		SDL_DestroyTexture(sdl->skybox);
	if (sdl->wall_surface)
		SDL_FreeSurface(sdl->wall_surface);
	if (sdl->wall_texture)
		SDL_DestroyTexture(sdl->wall_texture);
	sdl->window = NULL;
	sdl->renderer = NULL;
	sdl->skybox = NULL;
	sdl->wall_surface = NULL;
	sdl->wall_texture = NULL;
	SDL_Quit();
}

/**
 * create_renderer - creates instance
 * @sdl: sdl_instance
 * Return: nothing
 */
void create_renderer(sdl_instance *sdl)
{
	sdl->renderer = SDL_CreateRenderer(sdl->window, -1,
	SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (!sdl->renderer)
	{
		print_sdl_error();
		safe_close_sdl(sdl);
		exit(EXIT_FAILURE);
	}
}
/**
 * create_window - creates  instance
 * @name: string title
 * @sdl: data structure
 * Return: none
 */
void create_window(char *name, sdl_instance *sdl)
{
	sdl->window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (!sdl->window)
	{
		print_sdl_error();
		safe_close_sdl(sdl);
		exit(EXIT_FAILURE);
	}
}

/**
 * print_sdl_error - prints any sdl error
 * Return: nothing
 */
void print_sdl_error(void)
{
	printf("SDL Error Occured:%s\n", SDL_GetError());
}
