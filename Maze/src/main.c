#include "../headers/maze.h"

/**
 * main - Entry point
 * @argc: Number of arguments
 * @argv: Pointer to string
 * Return: (0) on success
 */
int main(int argc, char **argv)
{
	sdl_instance sdl = {NULL, NULL, NULL, NULL, NULL};
	map_t map;
	char *map_path;

	if (argc < 2)
	{
		printf("Usage: %s 'path'", argv[0]);
		exit(EXIT_FAILURE);
	}
	map_path = concat("maps/", argv[1]);
	map = handle_file(map_path);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("could not initialize! Error:%s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	create_window(WINDOW_TITLE, &sdl);
	create_renderer(&sdl);

	game_loop(&sdl, &map);
	free_map(&map);
	safe_close_sdl(&sdl);

	return (0);
}
