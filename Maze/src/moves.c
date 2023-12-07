#include "../headers/maze.h"

/**
 * rotate_player - listens to mouse
 * @player: datastructure
 * @mouse: data structure
 * Return: nothing
 */
void rotate_player(player *player, SDL_Point *mouse)
{
	int initial_x_pos = mouse->x;
	int delta = 0;

	SDL_GetMouseState(&mouse->x, &mouse->y);
	delta = mouse->x - initial_x_pos;
	if (delta < 0)
		player->view_angle -= ROTATION_MAGNITUDE;
	else
		player->view_angle += ROTATION_MAGNITUDE;
}

/**
 * move_player - Moving the player
 * @player: data structure holding
 * Return: SDL_Point containing
 */
SDL_Point move_player(player *player)
{
	SDL_Point displacement = {0, 0};
	double deg = player->view_angle;

	displacement.x = cos(RADIAN((90 - (deg - (FOV / 2))))) * MOVE_SPEED;
	displacement.y = sin(RADIAN((90 - (deg - (FOV / 2))))) * MOVE_SPEED;

	return (displacement);
}

/**
 * quadrant_of_angle - calculates the quadrant
 * @angle: check for it's quadrant
 * @resulting_angle: range between 0-360
 * Return: quadrant in which angle
 */
int quadrant_of_angle(int angle, int *resulting_angle)
{
	int quadrant;

	angle %= 360;
	if (angle < 0)
		angle += 360;
	quadrant = (angle / 90) % (4 + 1);
	*resulting_angle = angle;

	return (quadrant);
}

/**
 * slide_on_wall - slides the player
 * @player: pointer to data structure
 * Return: nothing
 */
void slide_on_wall(player *player)
{
	int angle;

	quadrant_of_angle((player->view_angle - (FOV / 2.0)), &angle);

	if (angle > 300 || angle < 60)
	{
		player->locale.x++;
		player->locale.y++;
	}
	else if (angle > 60 && angle < 170)
	{
		player->locale.x--;
		player->locale.y--;
	}
	else if (angle > 170 && angle < 235)
	{
		player->locale.x++;
		player->locale.y--;
	}
	else if (angle > 235 && angle < 300)
	{
		player->locale.x++;
		player->locale.y++;
	}
}

/**
 * draw_player - draws the player
 * @sdl: data structure
 * @player: data structure
 * Return: nothing
 */
void draw_player(sdl_instance *sdl, player *player)
{
	REND_COLOR_YELLOW(sdl->renderer);
	SDL_RenderFillRect(sdl->renderer, &player->locale);
}
