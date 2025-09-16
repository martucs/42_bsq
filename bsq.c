#include "bsq.h"

#include <stdio.h>

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
	fprintf(stdout, "%s", line);
	
	data->line_quantity = line[0];
	data->empty_char = line[2];
	data->full_char = line[4];
	
	// check que los 3 caracteres sean diferentes
	// que esten separados por un espacio
	int  i = 0;
	while (line[i])
	{
		i++;
	}
	

	return (map);
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

int	main(int argc, char **argv)
{
	t_info	data;
	if (argc == 1)
	{
		// read from input line by line, store in data;
		// find_bsq(data);
	}
	if (argc != 1)
	{
		for (int i = 1; i < argc; i++)
		{
			if (is_valid_file(argv[i]))
			{
				char **map = read_file(argv[i], &data);
				// if (valid_map)
					// find_bsq(data)
			}
		}
	}
	return (0);
}
