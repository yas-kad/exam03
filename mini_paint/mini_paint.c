#include "./mini_paint.h"

int		ft_strlen(char *str)
{
	int		len;

	len = 0;
	while (*str++)
		len++;
	return (len);
}

int		print_error(int error_id)
{
	if (error_id == ARGS_ID)
		write(STDOUT_FILENO, ARGS_ERR_MSG, ft_strlen(ARGS_ERR_MSG));
	else if (error_id == FILE_ID)
		write(STDOUT_FILENO, FILE_ERR_MSG, ft_strlen(FILE_ERR_MSG));
	return (1);
}

int		open_file(FILE **op_file, char *file_name)
{
	*op_file = fopen(file_name, "r");
	if (*op_file == NULL)
		return (ERROR);
	return (1);
}

int		get_zone_data(t_zone *zone, FILE *op_file)
{
	char	following_char;
	int		ret_code;

	following_char = '\0';
	if (fscanf(op_file, "%d %d %c", &zone->width, &zone->height, &zone->char_to_print) == 3)
	{
		ret_code = fscanf(op_file, "%c", &following_char);
		if (ret_code == EOF || (ret_code == 1 && following_char == '\n'))
		{
			if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->width > 300)
				return (ERROR);
			zone->data_arr = malloc(sizeof(char) * zone->width * zone->height);
			memset(zone->data_arr, zone->char_to_print, zone->width * zone->height);
		}
		else
			return (ERROR);
	}
	else
		return (ERROR);
	return (1);
}

void	drawing_time(t_zone *zone)
{
	int		x;
	
	x = 0;
	while (x < zone->width * zone->height)
	{
		write(STDOUT_FILENO, zone->data_arr + x, zone->width);
		x += zone->width;
		write(STDOUT_FILENO, "\n", 1);
	}
}

void	initialize_zone_vars(t_zone *zone)
{
	zone->char_to_print = '\0';
	zone->data_arr = NULL;
	zone->height = 0;
	zone->width = 0;
}

int		check_if_pixel_in_circle(float x, float y, t_circle *circle)
{
	float	distance;

	distance = sqrtf(powf(x - circle->x, 2) + powf(y - circle->y, 2));
	if (distance <= circle->radius)
		return (1);
	return (0);
}

int		check_if_pixel_in_boarder(float x, float y, t_circle *circle)
{
	float	distance;

	distance = sqrtf(powf(x - circle->x, 2) + powf(y - circle->y, 2));
	if (circle->radius - distance < (float)1)
		return (1);
	return (0);
}

int		store_curr_circle_data(t_zone *zone, t_circle *circle)
{
	int		x;
	int		y;

	y = 0;
	if (circle->radius <= 0)
		return (ERROR);
	while (y < zone->height)
	{
		x = 0;
		while (x < zone->width)
		{
			if (check_if_pixel_in_circle(x, y, circle) == 1)
				if (circle->type == 'C' || check_if_pixel_in_boarder(x, y, circle) == 1)
				zone->data_arr[(y * zone->width) + x] = circle->char_to_print;
			x++;
		}
		y++;
	}
	return (1);
}

int		get_circles_data(t_zone *zone, FILE *op_file)
{
	t_circle	circle;
	char		following_char;
	int			ret_code;

	ret_code = 0;
	following_char = '\0';
	while ((ret_code = fscanf(op_file, "%c %f %f %f %c", &circle.type, &circle.x,
		&circle.y, &circle.radius, &circle.char_to_print)) == 5)
	{
		ret_code = fscanf(op_file, "%c", &following_char);
		if (ret_code == EOF || (ret_code == 1 && following_char == '\n'))
		{
			if (store_curr_circle_data(zone, &circle) == ERROR)
				return (ERROR);
		}
		else
			return (ERROR);
	}
	if (ret_code != EOF && circle.type != '\n')
		return (ERROR);
	return (1);
}

int		main(int argc, char **argv)
{
	t_zone	zone;
	FILE	*op_file;

	if (argc == 2)
	{
		initialize_zone_vars(&zone);
		if (open_file(&op_file, argv[1]) == ERROR)
			return (print_error(FILE_ID));
		if (get_zone_data(&zone, op_file) == ERROR)
			return (print_error(FILE_ID));
		if (get_circles_data(&zone, op_file) == ERROR)
			return (print_error(FILE_ID));
		drawing_time(&zone);
	}
	else
		return (print_error(ARGS_ID));
	return (0);
}