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

void	read_line_by_line(t_info *data)
{
/*	int n = fscanf(stdin, "%d %c %c %c \n", &data->line_quantity, &data->empty_char, &data->obstacle_char, &data->full_char);
	if (n != 4)
	{
		fprintf(stdout, "Error: wrong map\n");
		return ;
	}*/
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

	// check que los 3 caracteres sean diferentes
	if (line[1] == line[2] || line[1] == line[3] || line[2] == line[3] || line[4])
	{
		fprintf(stdout, "chars are repeated or too many\n");
		return;
	}
	
	fprintf(stdout, "first line: %s\n", line);

	data->line_quantity = ft_chartoi(line[0]);
	data->empty_char = line[1];
	data->obstacle_char = line[2];
	data->full_char = line[3];

	fprintf(stdout, "line quantity = %d\n", data->line_quantity);
	fprintf(stdout, "empty char = %c\n", data->empty_char);
	fprintf(stdout, "obst char = %c\n", data->obstacle_char);
	fprintf(stdout, "full char = %c\n", data->full_char);
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

	size_t read_chars =  getline(&line, &len, filestream);
	if (read_chars == -1)
	{
		fclose(filestream);
		fprintf(stdout, "file is empty?\n");
		return (NULL);
	}
	fprintf(stdout, "%s", line);
	
	data->line_quantity = ft_chartoi(line[0]);
	data->empty_char = line[2];
	data->obstacle_char = line[4];
	data->full_char = line[6];
	
	// check que los 3 caracteres sean diferentes
	if (line[2] == line[4] || line[2] == line[6] || line[4] == line[6])
	{
		fprintf(stdout, "chars are repeated\n");
		return (NULL);
	}
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


void	find_bsq(t_info *data)
{
	char **new_map = NULL;
	int	square_len_size = data->line_len;
	int	square_height_size = data->line_quantity;

	for (int y = 0; y < data->line_quantity - square_height_size; y++)
	{
		for (int x = 0; x < data->line_len - square_len_size; x++)
		{
			if (data->map[y][x] == data->obstacle_char)
			{
				square_len_size = x;
				break;
			}
		}
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
					find_bsq(&data);
			}
		}
	}
	else
		fprintf(stdout, "Usage: ./a.out [file]\n");
	return (0);
}
