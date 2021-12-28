#ifndef MINI_PAINT
# define MINI_PAINT

# include <math.h>
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <stdlib.h> 

# define ERROR -1
# define ARGS_ID 0
# define FILE_ID 1
# define ARGS_ERR_MSG "Error: argument\n"
# define FILE_ERR_MSG "Error: Operation file corrupted\n"

typedef struct s_zone
{
	int		width;
	int		height;
	char	char_to_print;
	char	*data_arr;
}				t_zone;

typedef struct s_circle
{
	char	type;
	float	x;
	float	y;
	float	radius;
	char	char_to_print;
}				t_circle;

#endif