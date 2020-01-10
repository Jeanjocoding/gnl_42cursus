#include "get_next_line.h"
#include <stdio.h>

int		main(int argc, char **argv)
{
	char	*line;
	int		fd;
	int		fd2;
	int		ret;

	if (argc == 3)
	{
		fd = open(argv[1], O_RDONLY);
		fd2 = open(argv[2], O_RDONLY);
	}
	else
		fd = 0;
	line = NULL;
	ret = 1;
	while (ret && ret != -1)
	{
		ret = get_next_line(fd, &line);
		printf("%s\treturn value = %d\n", line, ret);
		ret = get_next_line(fd2, &line);
		printf("%s\treturn value = %d\n", line, ret);
		free(line);
		line = NULL;
	}
	close(fd);
}

