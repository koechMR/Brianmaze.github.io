#include "../headers/maze.h"



/**
 * textured_sky - sky from a bmp file
 * @sdl: sdl_instance
 * @map: data structure
 * Return: nothing
 */
void textured_sky(sdl_instance *sdl, map_t *map)
{
	SDL_Rect sky_dome = {0, 0, SCREEN_WIDTH, (SCREEN_HEIGHT >> 1)};
	SDL_Surface *surface;

	if (!sdl->skybox)
	{
		surface = SDL_LoadBMP("images/skypic.bmp");
		if (!surface)
		{
			printf("Error: %s", SDL_GetError());
			free_map(map);
			safe_close_sdl(sdl);
			exit(EXIT_FAILURE);
		}
		sdl->skybox = SDL_CreateTextureFromSurface(sdl->renderer, surface);
		SDL_FreeSurface(surface);
	}
	SDL_RenderCopy(sdl->renderer, sdl->skybox, NULL, &sky_dome);
}

/**
 * render_thread - A thread to handle rendering
 * @td: void pointer
 * Description: necessary since the blitting
 * Return: integer for the thread status
 */
int render_thread(void *td)
{
	thread_data *data = td;
	Uint64 start, end;
	double elapsedMs;

	while (!(*data->quit))
	{
		start = SDL_GetPerformanceCounter();

		textured_sky(data->sdl, data->map);
		untextured_floor(data->sdl);
		raycast(data->sdl, data->player, data->map, data->map_active);
		if (*data->map_active)
		{
			draw_2d_map(data->sdl, data->map);
			draw_player(data->sdl, data->player);
		}
		send_frame(data->sdl);

		end = SDL_GetPerformanceCounter();
		elapsedMs = (end - start) / SDL_GetPerformanceCounter() * 1000;
		SDL_Delay(floor(16.666 - elapsedMs));
	}
	return (0);
}

/**
 * texture_walls - draws walls with textures
 * @sdl: data structure of sdl_instance
 * @line_height: Height of the to draw
 * @column: current column on screen to draw texture
 * @y_pos: The y coordinate to start at @column
 * @alpha: controls darkening
 * Return: nothing
 */
void texture_walls(sdl_instance *sdl, double line_height, int column,
		int y_pos, uint8_t alpha)
{
	SDL_Rect dest = {0, 0, 1, 64};
	SDL_Rect src_rect = {(column % 64), 0, 1, 64};
	int result;
	SDL_Surface *dest_surface = SDL_CreateRGBSurfaceWithFormat(0, 1,
			64, 32, SDL_GetWindowPixelFormat(sdl->window));
	SDL_Rect rend_rect = {column, y_pos, 1, (line_height)};

	if (!sdl->wall_surface)
	{
		sdl->wall_surface = SDL_LoadBMP("images/brick.bmp");
		if (!sdl->wall_surface)
		{
			printf("Error: %s\n", SDL_GetError());
			exit(EXIT_FAILURE);
		}
	}
	SDL_UnlockSurface(dest_surface);
	result = SDL_BlitSurface(sdl->wall_surface, &src_rect, dest_surface, &dest);
	SDL_LockSurface(dest_surface);
	if (result < 0)
	{
		printf("Error: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	if (!sdl->wall_texture)
	{
		sdl->wall_texture = SDL_CreateTextureFromSurface(sdl->renderer,
				dest_surface);
		if (!sdl->wall_texture)
		{
			printf("Error: %s\n", SDL_GetError());
			exit(EXIT_FAILURE);
		}
	}
	SDL_UpdateTexture(sdl->wall_texture, NULL, dest_surface->pixels,
			dest_surface->pitch);
	SDL_SetTextureColorMod(sdl->wall_texture, alpha, alpha, alpha);
	SDL_RenderCopy(sdl->renderer, sdl->wall_texture, NULL, &rend_rect);
	SDL_FreeSurface(dest_surface);
}
/**
 * shade_walls - walls based on orientation
 * @color_ref: Pointer to color
 * @orientation: Side in which the ray is hitting
 * Return: nothing
 */
void shade_walls(SDL_Color *color_ref, int orientation)
{
	SDL_Color up_down = {112, 112, 112, 255};
	SDL_Color left_right = {96, 96, 96, 150};

	if (orientation == ORIENT_UP_DOWN)
		*color_ref = up_down;
	else
		*color_ref = left_right;
}
