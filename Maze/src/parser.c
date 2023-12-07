#include "../headers/maze.h"

/**
 * validate_line_data - check for errors
 * @max_line_count: check maximum
 * @line: line
 * @fp: pointer
 * @line_number: current line
 * Return: nothing
 */
void validate_line_data(int max_line_count, char *line, FILE *fp,
		int line_number)
{
	char valid_digits[] = {'0', '1'};
	int a;
	unsigned int line_length = strlen(line);

	if (!line)
		return;
	if (line_length != (unsigned int)max_line_count)
	{
		free(line);
		close_file(fp);
		printf("Error line No. %d: The columns of the map should be equal.\n",
				line_number);
		exit(EXIT_FAILURE);
	}

	for (a = 0; a < max_line_count; a++)
	{
		if ((in_array(line[a], valid_digits, max_line_count)) == 0)
		{

			printf("Error line No. %d: Invalid map values found.\n", line_number);
			exit(EXIT_FAILURE);
		}
	}
}

/**
 * in_array - check
 * @needle: value to look
 * @stack: Pointer to string
 * @stack_size: array @haystack
 * Return: (0) if value not in array otherwise (1)
 */
int in_array(char needle, char *stack, unsigned int stack_size)
{
	unsigned int c;

	for (c = 0; c < stack_size; c++)
	{
		if (stack[c] == needle)
			return (1);
	}

	return (0);
}

/**
 * concat - concatenates two strings
 * @s1: destination
 * @s2: source string
 * Return: New string with @s1 and @s2
 */
char *concat(const char *s1, const char *s2)
{
	char *result = NULL;

	result = malloc(strlen(s1) + strlen(s2) + 1);
	if (!result)
		return (NULL);

	strcpy(result, s1);
	strcat(result, s2);
	return (result);
}

/**
 * count_rows - counts
 * @fp: File pointer
 * Return: number
 */
int count_rows(FILE *fp)
{
	char ch;
	int lines = 0;

	while (!feof(fp))
	{
		ch = fgetc(fp);
		if (ch == '\n')
		{
			lines++;
		}
	}

	fseek(fp, 0, SEEK_SET);

	return (lines + 1);
}

/**
 * free_map - frees 2D map
 * @map: data structure
 * Return: nothing
 */
void free_map(map_t *map)
{
	int x;

	for (x = 0; x < map->rows; x++)
		free(map->arr[x]);
	free(map->arr);
}
