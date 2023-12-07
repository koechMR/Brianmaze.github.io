#include "../headers/maze.h"

/**
 * events - listens
 * @quit: pointer to integer
 * @z: pointer to SDL_Event
 * @player: pointer to data structure
 * @mouse: Pointer
 * @map_active: Boolean
 * Return: Nothing
 */
void events(int *quit, SDL_Event *z, player *player, SDL_Point *mouse,
		SDL_bool *map_active)
{
	SDL_Point displacement = {0, 0};

	displacement = move_player(player);
	while (SDL_PollEvent(z) != 0)
	{
		if (z->type == SDL_QUIT)
			*quit = 1;
		if (z->type == SDL_MOUSEMOTION)
			rotate_player(player, mouse);

		switch (z->key.keysym.sym)
		{
		case SDLK_ESCAPE:
			*quit = 1;
			break;
		case SDLK_w:
			player->locale.x += displacement.x;
			player->locale.y -= displacement.y;
			break;
		case SDLK_s:
			player->locale.x -= displacement.x;
			player->locale.y += displacement.y;
			break;
		case SDLK_a:
			player->view_angle -= ROTATION_MAGNITUDE;
			break;
		case SDLK_d:
			player->view_angle += ROTATION_MAGNITUDE;
			break;
		case SDLK_m:
			*map_active = SDL_FALSE;
			break;
		case SDLK_n:
			*map_active = SDL_TRUE;
			break;
		default:
			break;
		}
	}
}

/**
 * game_loop - handles SDL rendering
 * @sdl: data structure
 * @map: map_t data structure
 * Return: null
 */
void game_loop(sdl_instance *sdl, map_t *map)
{
	int quit = 0;
	SDL_Event a;
	player player = {{80, 70, PLAYER_WIDTH, PLAYER_WIDTH}, (FOV * 2.5)};
	SDL_Point mouse = {0, 0};
	SDL_bool map_active = SDL_TRUE;
	thread_data data = {sdl, map, &player, &map_active, &quit};
	SDL_Thread *thread_ID = NULL;

	thread_ID = SDL_CreateThread(render_thread, "RenderThread", &data);

	while (!quit)
	{
		events(&quit, &a, &player, &mouse, &map_active);
		player_collision_detection(&player, map);
	}
	SDL_WaitThread(thread_ID, NULL);
}
