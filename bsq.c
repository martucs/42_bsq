#include "bsq.h"
#include <stdlib.h>
#include <stdio.h>

int	ft_strlen(char *str)
{
	int i = 0;
	
	while (str[i])
		i++;
	return (i);
}

int	is_digit(unsigned char c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

void	print_arr(char **arr)
{
	int x = 0;
	while (arr && arr[x])
	{
		fprintf(stdout, "%s\n", arr[x]);
		x++;
	}
}

void	free_arr(char **arr)
{
	int i = 0;
	if (!arr)
		return ;
	while (arr && arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	**read_line_by_line(t_info *data)
{
	char	**map = NULL;
	char	*line = NULL;
	size_t	len = 0;;

	int read = getline(&line, &len, stdin);
	if (read == -1)
	{
		free(line);
		fprintf(stderr, "Error: getline error\n");
		return (NULL);
	}
	if (line[read - 1] == '\n')
	{
		line[read - 1] = '\0';
		read--;
	}
	int number = 0;
	int pos = 0;
	// read digits
	while (is_digit((unsigned char)line[pos])) 
	{
		number = number * 10 + (line[pos] - '0');
		pos++;
	}

	if (pos == 0 || number <= 0)
	{
		fprintf(stderr, "Error: map error\n");
		free(line);
		return (NULL) ;
	}
	// must have exactly 3 more chars
	if (read - pos != 3)
	{
		fprintf(stderr, "Error: map error\n");
		free(line);
		return (NULL) ;
	}
	// check que los 3 caracteres sean diferentes
	if (line[pos] == line[pos + 1] || line[pos] == line[pos + 2] || line[pos + 1] == line[pos + 2])
	{
		free(line);
		fprintf(stdout, "Error: map error\n");
		return (NULL);
	}
	data->line_quantity = number;
	data->empty_char    = line[pos];
	data->obstacle_char = line[pos + 1];
	data->full_char     = line[pos + 2];

	free(line);

	map = malloc((data->line_quantity + 1) * sizeof(char *));
	for (int x = 0; x <= data->line_quantity; x++)
		map[x] = NULL;

	for (int i = 0; i < data->line_quantity; i++)
	{
		char *line = NULL;
		size_t len = 0;
		if (getline(&line, &len, stdin) == -1)
		{
			free_arr(map);
			free(line);
			fprintf(stderr, "Error: getline error\n");
			return (NULL);
		}
		// Remove newline
		int line_len = ft_strlen(line);
		if (line[line_len - 1] == '\n')
		{
			line[line_len - 1] = '\0';
			line_len--;
		}
		if (i == 0)
			data->line_len = line_len;
		else if (ft_strlen(line) != data->line_len)
		{
			free(line);
			if (map)
				free_arr(map);
			fprintf(stdout, "Error: map error\n");
			return (NULL);
		}
		map[i] = line;
	}
	// check de que no haya caracteres diferentes a 'empty' y 'obst'
	for (int y = 0; y < data->line_quantity; y++)
	{
		for (int x = 0; x < data->line_len;x++)
			if (map[y][x] != data->empty_char && map[y][x] != data->obstacle_char)
			{
				fprintf(stderr, "Error: map error5\n");
				free_arr(map);
				return (NULL);
			}
	}
	return (map);
}

char	**read_file(char *filename, t_info *data)
{
	FILE *filestream = fopen(filename, "r");
	if (!filestream)
	{
		fprintf(stderr, "Error: could not open \'%s\'\n", filename);
		return (NULL);
	}	
	
	char	**map = NULL;
	char	*line = NULL;
	size_t	len = 0;

	int read = getline(&line, &len, filestream);
	if (read == -1)
	{
		fclose(filestream);
		free(line);
		return (NULL);
	}
	if (line[read - 1] == '\n')
	{
		line[read - 1] = '\0'; // quitamos el '\'
		read--;
	}
	
	int number = 0;
	int pos = 0;
	while (is_digit((unsigned char)line[pos])) 
	{
		number = number * 10 + (line[pos] - '0');
		pos++;
	}
	if (pos == 0 || number <= 0)
	{
		fprintf(stderr, "Error: map error\n");
		free(line);
		fclose(filestream);
		return (NULL);
	}
	// must have exactly 3 more chars
	if (read - pos != 3)
	{
		fprintf(stderr, "Error: map error\n");
		free(line);
		fclose(filestream);
		return (NULL);
	}
	// check que los 3 caracteres sean diferentes
	if (line[pos] == line[pos + 1] || line[pos] == line[pos + 2] || line[pos + 1] == line[pos + 2])
	{
		free(line);
		fprintf(stdout, "Error: map error\n");
		fclose(filestream);
		return (NULL);
	}

	data->line_quantity = number;
	data->empty_char    = line[pos];
	data->obstacle_char = line[pos + 1];
	data->full_char     = line[pos + 2];

	free(line);
	
	map = malloc((data->line_quantity + 1) * sizeof(char *));
	if (!map)
	{
		fclose(filestream);
		fprintf(stdout, "Error: malloc error\n");
		return (NULL);
	}
	for (int x = 0; x <= data->line_quantity; x++)
		map[x] = NULL;
	data->line_len = 0;

	int i = 0;
	while (1) 
	{
		char *line = NULL;
		size_t len = 0;
		if (getline(&line, &len, filestream) == -1)
		{
			free(line);
			break;
		}
		if (i >= data->line_quantity)
		{
			free(line);
			free_arr(map);
			fclose(filestream);
			fprintf(stderr, "Error: map has more lines than specified\n");
			return (NULL);
		}
		// Remove newline
		int line_len = ft_strlen(line);
		if (line[line_len - 1] == '\n')
		{
			line[line_len - 1] = '\0';
			line_len--;
		}
		if (i == 0) 
			data->line_len = line_len;
		else if (line_len != data->line_len)
		{
			fprintf(stderr, "Error: map error\n");
			free(line);
			free_arr(map);
			fclose(filestream);
			return (NULL);
		}
		map[i] = line;
		i++;
	}
	fclose(filestream);
	if (i != data->line_quantity) 
	{
		free_arr(map);
		fprintf(stderr, "Error: map has fewer lines than specified\n");
		return (NULL);
	}
	// check de que no haya caracteres diferentes a 'empty' y 'obst'
	for (int y = 0; y < data->line_quantity; y++)
	{
		for (int x = 0; x < data->line_len;x++)
			if (map[y][x] != data->empty_char && map[y][x] != data->obstacle_char)
			{
				fprintf(stderr, "Error: map error\n");
				free_arr(map);
				return (NULL);
			}
	}
	return (map);
}

void	print_filled_map(char **map, t_info data)
{
	int x, y = 0;

	while (y < data.line_quantity)
	{
		x = 0;
		while (x < data.line_len)
		{
			if (x >= data.sq_x_start && x < data.sq_x_start + data.sq_dimension &&
				y >= data.sq_y_start && y < data.sq_y_start + data.sq_dimension)
				fprintf(stdout, "%c", data.full_char);
			else
				fprintf(stdout, "%c", data.map[y][x]);
			x++;
		}
		y++;
		fprintf(stdout, "\n");
	}
}

int	check_square(char **map, t_info data, int x_start, int y_start, int dimension)
{
	int y, x;

	y = 0;
	while (y < dimension /*&& y + y_start < data.line_quantity*/)
	{
		x = 0;
		while (x < dimension /*&& x + x_start < data.line_len*/)
		{
			if (data.map[y_start + y][x_start + x] == data.obstacle_char)
				return (0);
			x++;
		}
		y++;
	}
	return (1);
}

void	find_bsq(t_info *data)
{
	char **new_map = NULL;
	int	square_len_size = data->line_len;
	int	square_height_size = data->line_quantity;
	int	dimension = square_len_size > square_height_size ? square_height_size : square_len_size;

	int y, x;
	while (dimension > 0)
	{
		y = 0;
		while(y + dimension <= square_height_size)
		{
			x = 0;
			while(x + dimension <= square_len_size)
			{
				if(check_square(data->map, *data, x, y, dimension) == 1)
				{
					data->sq_dimension = dimension;
					data->sq_x_start = x;
					data->sq_y_start = y;
					return ;
				}
				x++;
			}
			y++;
		}
		dimension--;
	}
}


int	is_valid_file(char *filename)
{
	FILE *filestream = fopen(filename, "r");
	if (!filestream)
	{
		fprintf(stderr, "Error: could not open \'%s\'\n", filename);
		return (0);
	}
	fclose(filestream);
	return (1);
}

void	init_data(t_info *data)
{
	data->line_quantity = 0;
	data->line_len = 0;
	data->empty_char = '\0';
	data->obstacle_char = '\0';
	data->full_char = '\0';
	data->map = NULL;
	data->sq_dimension = 0;
	data->sq_x_start = 0;
	data->sq_y_start = 0;
}

int	main(int argc, char **argv)
{
	t_info	data;

	init_data(&data);
	if (argc == 1)
	{
		data.map = read_line_by_line(&data);
		if (data.map)
		{	
			find_bsq(&data);
			fprintf(stdout, "\n");
			print_filled_map(data.map, data);
			free_arr(data.map);
		}
	}
	else if (argc == 2)
	{
		if (is_valid_file(argv[1]))
		{
			data.map = read_file(argv[1], &data);
			if (data.map)
			{
				find_bsq(&data);
				print_filled_map(data.map, data);
				free_arr(data.map);
			}
		}
	}
	else
		fprintf(stdout, "Usage: ./a.out [file]\n");
	return (0);
}
