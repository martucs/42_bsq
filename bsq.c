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

int	ft_chartoi(char c)
{
	int res;

	res = 0;
	int i = 0;
	if (c >= '0' && c <= '9')
	{
		res = res * 10 + (c - '0');
		i++;
	}
	return (res);
}

int	is_digit(unsigned char c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

void	read_line_by_line(t_info *data)
{
	char	*line = NULL;
	size_t	len = 0;;

	int read = getline(&line, &len, stdin);
	if (read == -1)
	{
		free(line);
		return;
	}

	// remove trailing newline if present
	if (line[read - 1] == '\n')
	{
		line[read - 1] = '\0';
		read--;
	}

	// parse: must be number + 3 chars, nothing else
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
		fprintf(stderr, "Error: invalid number of lines\n");
		free(line);
		return ;
	}

	// must have exactly 3 more chars
	if (read - pos != 3)
	{
		fprintf(stderr, "Error: header must have exactly 3 characters after the number\n");
		free(line);
		return ;
	}

	// check que los 3 caracteres sean diferentes
	if (line[pos] == line[pos + 1] || line[pos] == line[pos + 2] || line[pos + 1] == line[pos + 2] || line[pos + 3])
	{
		fprintf(stdout, "chars are repeated or too many\n");
		return;
	}

	fprintf(stdout, "first line: %s\n", line);

	data->line_quantity = number;
	data->empty_char    = line[pos];
	data->obstacle_char = line[pos + 1];
	data->full_char     = line[pos + 2];

	fprintf(stdout, "line quantity = %d\n", data->line_quantity);
	fprintf(stdout, "empty char = %c\n", data->empty_char);
	fprintf(stdout, "obst char = %c\n", data->obstacle_char);
	fprintf(stdout, "full char = %c\n", data->full_char);

/*	data->map = malloc(data->line_quantity * sizeof(char *) + 1);
	data->map[data->lie
	line = NULL;
	size_t len = 0;

	for (int i = 0; i < rows; i++) {
		if (getline(&line, &len, stdin) == -1) {
			fprintf(stderr, "Error reading line %d\n", i);
			break;
		}
		fprintf(stdout, "Line %d: %s", i, line);
	}

	free(line);*/
}

char	**read_file(char *filename, t_info *data)
{
	char	**map;
	FILE *filestream = fopen(filename, "r");
	
	map = NULL;
	char	*line;
	size_t	len;

	line = NULL;
	len = 0;

	int read = getline(&line, &len, filestream);
	if (read == -1)
	{
		fclose(filestream);
		free(line);
		return (NULL);
	}
	// remove trailing newline if present
	if (line[read - 1] == '\n')
	{
		line[read - 1] = '\0';
		read--;
	}

	// parse: must be number + 3 chars, nothing else
	int number = 0;
	int pos = 0;
	while (is_digit((unsigned char)line[pos])) 
	{
		number = number * 10 + (line[pos] - '0');
		pos++;
	}

	if (pos == 0 || number <= 0)
	{
		fprintf(stderr, "Error: invalid number of lines\n");
		free(line);
		fclose(filestream);
		return (NULL);
	}

	// must have exactly 3 more chars
	if (read - pos != 3)
	{
		fprintf(stderr, "Error: header must have exactly 3 characters after the number\n");
		free(line);
		fclose(filestream);
		return (NULL);
	}

	// check que los 3 caracteres sean diferentes
	if (line[pos] == line[pos + 1] || line[pos] == line[pos + 2] || line[pos + 1] == line[pos + 2] || line[pos + 3])
	{
		fprintf(stdout, "chars are repeated or too many\n");
		fclose(filestream);
		return (NULL);
	}

	fprintf(stdout, "first line: %s\n", line);

	data->line_quantity = number;
	data->empty_char    = line[pos];
	data->obstacle_char = line[pos + 1];
	data->full_char     = line[pos + 2];

	map = malloc(data->line_quantity * sizeof(char *));
	// protect malloc
	data->line_len = 0;

	for (int i = 0; i < data->line_quantity; i++) 
	{
		char *line = NULL;
		size_t len = 0;
		if (getline(&line, &len, filestream) == -1)
		{
			free(line);
			free(map);
			fclose(filestream);
			fprintf(stdout, "someting went wrong with getline\n");
			return (NULL);
		}

		// Remove newline
		int line_len = ft_strlen(line);
		if (line[line_len - 1] == '\n')
			line[line_len - 1] = '\0';

		if (i == 0)
			data->line_len = line_len - 1;
		else if (ft_strlen(line) != data->line_len)
		{
			//free all map char**
			free(map);
			fclose(filestream);
			fprintf(stdout, "map line lengths dont match\n");
			return (NULL);
		}
		map[i] = line;
		if (i == 0)
			fprintf(stdout, "line_len = %d\n", data->line_len);
		fprintf(stdout, "storing %s\n", line);
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
	while (y < dimension)
	{
		x = 0;
		fprintf(stdout, "y = %d\n", y + y_start);
		while (x < dimension)
		{
			fprintf(stdout, "x = %d\n", x + x_start);
			if (data.map[y_start + y][x_start + x] == data.obstacle_char)
			{
				fprintf(stdout, "found obstacle at x = %d, y = %d\n", x + x_start, y + y_start);
				return (0);
			}
			x++;
		}
		y++;
	}
	return (1);
}

// mirar si hay como minimo un caracter 'vacio' en el mapa, sino no podriamos hacer cuadrado

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
		fprintf(stdout, "dimension = %d\n", dimension);
		while(y + dimension <= square_height_size)
		{
			fprintf(stdout, "vert = %d\n", y);
			x = 0;
			while(x + dimension <= square_len_size)
			{
				fprintf(stdout, "hor = %d\n", x);
				if(check_square(data->map, *data, x, y, dimension) == 1)
				{
					fprintf(stdout, "found square at x: %d, y: %d with dimension %d!\n", x, y, dimension);
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
		read_line_by_line(&data);
		// find_bsq(data);
	}
	else if (argc == 2)
	{
		for (int i = 1; i < argc; i++)
		{
			if (is_valid_file(argv[i]))
			{
				data.map = read_file(argv[i], &data);
				if (data.map)
				{
					find_bsq(&data);
					print_filled_map(data.map, data);
				}
			}
		}
	}
	else
		fprintf(stdout, "Usage: ./a.out [file]\n");
	return (0);
}
