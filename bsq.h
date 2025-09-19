#ifndef BSQ_H
#define BSQ_H

typedef struct	s_info
{
	int	line_quantity;
	int	line_len;
	char	empty_char;
	char	obstacle_char;
	char	full_char;
	char	**map;
	int	sq_dimension;
	int	sq_x_start;
	int	sq_y_start;
}	t_info;

#endif
