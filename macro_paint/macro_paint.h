#ifndef MACRO_PAINT_H
# define MACRO_PAINT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

# define ERROR -1
# define ARGS_CODE 0
# define FILE_CODE 1
# define ARGS_ERR_MSG "Error: argument\n"
# define FILE_ERR_MSG "Error: Operation file corrupted\n"

typedef struct s_zone
{
	int		width;
	int		height;
	char	char_to_draw;
	char	*data_array;
}				t_zone;

typedef struct s_rect
{
	char	type;
	float	x;
	float	y;
	float	width;
	float	height;
	char	char_to_draw;
}				t_rect;

#endif
