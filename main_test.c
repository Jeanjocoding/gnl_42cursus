#include "get_next_line.h"
#include <stdio.h>

int		main(void)
{
	char	*str;
	int		fd;
	int		ret;

	fd = open("get_next_line.c", O_RDONLY);
	while ((ret = get_next_line(fd, &str)))
	{
		if (ret == -1)
			return(printf("get_next_line exit with -1"));
		printf("str :  %s _  ret : %d\n", str, ret);
		free(str);
	}
	printf("%s\n", str);
	return (0);
}
