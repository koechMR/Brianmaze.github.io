#include "../headers/maze.h"

/**
 * player_collision_detection - detects collision
 * @player: pointer to data structure of player
 * @map: pointer
 * Return: nothing
 */
void player_collision_detection(player *player, map_t *map)
{
	SDL_Rect wall = {0, 0, GRID_SIZE, GRID_SIZE};
	int x, a;
	int border = 5;
	int dimensions = player->locale.w + border;
	SDL_Rect bounding_box = {player->locale.x - (border / 2),
		player->locale.y - (border / 2),
		dimensions, dimensions};

	for (x = 0; x < map->rows; x++)
	{
		for (a = 0; a < map->columns; a++)
		{
			if (map->arr[x][a] == '0')
				continue;
			wall.x = (a << 4) + MAP_MARGIN;
			wall.y = (x << 4) + MAP_MARGIN;
			if (SDL_HasIntersection(&bounding_box, &wall))
				slide_on_wall(player);
		}
	}
}
/**
 * untextured_floor - Draws untextured
 * @sdl: instance of sdl data structure
 * Return: nothing
 */
void untextured_floor(sdl_instance *sdl)
{
	double offset_y = (SCREEN_HEIGHT >> 1);
	SDL_Rect sky_dome = {0, offset_y, SCREEN_WIDTH, (SCREEN_HEIGHT >> 1)};

	REND_COLOR(sdl->renderer, 52, 140, 49, 255);
	SDL_RenderFillRect(sdl->renderer, &sky_dome);
}

/**
 * untextured_sky - Draws untextured sky
 * @sdl: instance of sdl data structure
 * Return: nothing
 */
void untextured_sky(sdl_instance *sdl)
{
	SDL_Rect sky_dome = {0, 0, SCREEN_WIDTH, (SCREEN_HEIGHT >> 1)};

	REND_COLOR(sdl->renderer, 25, 181, 254, 255);
	SDL_RenderFillRect(sdl->renderer, &sky_dome);
}

/**
 * check_intersect_orientation - line intersects
 * @wall: data structure with rectangle
 * @pointA: Pointer at beginning
 * @pointB: Pointer
 * Return: 1 success
 */
int check_intersect_orientation(SDL_Rect wall, SDL_Point *pointA,
		SDL_Point *pointB)
{
	line top = {{wall.x, wall.y}, {wall.x + wall.w, wall.y}};
	line bottom = {{wall.x, wall.y + wall.h}, {wall.x + wall.w, wall.y + wall.h}};
	line ray = {*pointA, *pointB};
	SDL_Point intersection = {0, 0};

	if (lines_intersect(&top, &ray, &intersection))
		return (ORIENT_UP_DOWN);
	else if (lines_intersect(&bottom, &ray, &intersection))
		return (ORIENT_UP_DOWN);
	return (ORIENT_LEFT_RIGHT);
}

/**
 * lines_intersect - Checks if two lines segments
 * @line1: Pointer to line structure line1
 * @line2: Pointer to line structure line2
 * @hitp: Ponter to sdl
 * Return: (1) if they intersect, (0) if they do not.
 */
int lines_intersect(line *line1, line *line2, SDL_Point *hitp)
{
	int d, n_a, n_b, ua, ub;

	d = (line2->p2.y - line2->p1.y) * (line1->p2.x - line1->p1.x) -
	(line2->p2.x - line2->p1.x) * (line1->p2.y - line1->p1.y);

	n_a = (line2->p2.x - line2->p1.x) * (line1->p1.y - line2->p1.y) -
	(line2->p2.y - line2->p1.y) * (line1->p1.x - line2->p1.x);

	n_b = (line1->p2.x - line1->p1.x) * (line1->p1.y - line2->p1.y) -
	(line1->p2.y - line1->p1.y) * (line1->p1.x - line2->p1.x);

	if (d == 0)
		return (0);

	ua = (n_a << 14) / d;
	ub = (n_b << 14) / d;

	if (ua >= 0 && ua <= (1 << 14) && ub >= 0 && ub <= (1 << 14))
	{
		hitp->x = line1->p1.x + ((ua * (line1->p2.x - line1->p1.x)) >> 14);
		hitp->y = line1->p1.y + ((ua * (line1->p2.y - line1->p1.y)) >> 14);
		return (1);
	}

	return (0);
}
