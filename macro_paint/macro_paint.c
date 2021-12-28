
#include "./macro_paint.h"

int		ft_strlen(char *str)
{
	int		len;

	len = 0;
	while (*str++)
		len++;
	return (len);
}


int		get_zone_data(t_zone *zone, FILE *op_file)
{
	int		ret_num;
	char	following_char;

	ret_num = 0;
	following_char = '\0';
	if (fscanf(op_file, "%d %d %c", &zone->width, &zone->height, &zone->char_to_draw) == 3)
	{
		ret_num = fscanf(op_file, "%c", &following_char);
		if (ret_num == EOF || (ret_num == 1 && following_char == '\n'))
		{
			if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
				return (ERROR);
			zone->data_array = malloc(sizeof(char) * zone->height * zone->width);
			memset(zone->data_array, zone->char_to_draw, zone->height * zone->width);
		}
		else
			return (ERROR);
	}
	else
		return (ERROR);
	return (0);
}

int	print_error(int error_code)
{
	if (error_code == FILE_CODE)
		write(STDOUT_FILENO, FILE_ERR_MSG, ft_strlen(FILE_ERR_MSG));
	else if (error_code == ARGS_CODE)
		write(STDOUT_FILENO, ARGS_ERR_MSG, ft_strlen(ARGS_ERR_MSG));
	return (1);
}

void	initialize_rect(t_rect *rect)
{
	rect->char_to_draw = '\0';
	rect->height = 0;
	rect->width = 0;
	rect->x = 0;
	rect->y = 0;
}

int		check_if_pix_in_rec(float x, float y, t_rect *curr_rect)
{
	float		right_most_pixel_x;
	float		down_most_pixel_y;

	right_most_pixel_x = curr_rect->x + curr_rect->width;
	down_most_pixel_y = curr_rect->y + curr_rect->height;
	if ((x >= curr_rect->x && x <= right_most_pixel_x)
	 && (y >= curr_rect->y && y <= down_most_pixel_y))
		return (1);
	return (0);
}

int		check_if_pix_in_border(float x, float y, t_rect *rect)
{
	float		right_most_pixel_x;
	float		down_most_pixel_y;
	right_most_pixel_x = rect->x + rect->width;
	down_most_pixel_y = rect->y + rect->height;
	if ((x - rect->x < (float)1 || right_most_pixel_x - x < (float)1) ||
		(y - rect->y < (float)1 || down_most_pixel_y - y < (float)1))
	 	return (1);
	return (0);
}

int		store_curr_rect_data(t_zone *zone, t_rect *curr_rect)
{
	if (curr_rect->width > 0 && curr_rect->height > 0 &&
	(curr_rect->type == 'r' || curr_rect->type == 'R'))
	{
		for (int x = 0; x < zone->width; x++)
			for (int y = 0; y < zone->height; y++)
			{
				if (check_if_pix_in_rec(x, y, curr_rect) == 1)
					if (curr_rect->type == 'R' ||  check_if_pix_in_border(x, y, curr_rect) == 1)
						zone->data_array[(y * zone->width) + x] = curr_rect->char_to_draw;
			}
	}
	else
		return (ERROR);
	return (0);
}

int		get_rects_data(t_zone *zone, FILE *op_file)
{
	t_rect	curr_rect;
	int		ret_num;
	char	following_char;

	ret_num = 0;
	following_char = '\0';
	initialize_rect(&curr_rect);
	while ((ret_num = fscanf(op_file, "%c %f %f %f %f %c", &curr_rect.type, &curr_rect.x,
	&curr_rect.y, &curr_rect.width, &curr_rect.height, &curr_rect.char_to_draw)) == 6)
	{
		ret_num = fscanf(op_file, "%c", &following_char);
		if (ret_num == EOF || (ret_num == 1 && following_char == '\n'))
		{
			if (store_curr_rect_data(zone, &curr_rect) == ERROR)
				return (ERROR);
		}
		else
			return (ERROR);
	}
	if (ret_num != EOF && curr_rect.type != '\n')
		return (ERROR);
	return (0);
}

void	initializ_vars(t_zone *zone)
{
	zone->height = 0;
	zone->width = 0;
	zone->data_array = NULL;
	zone->char_to_draw = '\0';
}

int		main(int argc, char **argv)
{
	FILE	*op_file;
	t_zone	zone;

	initializ_vars(&zone);
	if (argc == 2)
	{
		op_file = fopen(argv[1], "r");
		if (op_file == NULL)
			return (print_error(FILE_CODE));
		if (get_zone_data(&zone, op_file) == ERROR)
			return (print_error(FILE_CODE));
		if (get_rects_data(&zone, op_file) == ERROR)
			return (print_error(FILE_CODE));
		
		for (int i=0; i < zone.width * zone.height; i += zone.width)
		{
			write(STDOUT_FILENO, zone.data_array + i, zone.width);
			write(STDOUT_FILENO, "\n", 1);
		}
	}
	else
		return (print_error(ARGS_CODE));
	return (0);
}