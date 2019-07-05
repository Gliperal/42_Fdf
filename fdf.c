/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 20:36:57 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/04 23:00:32 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "map.h"

t_map	*read_map(int fd);
void	fdf(t_map *map);

int	main(int argc, char **argv)
{
	int		fd;
	t_map	*map;

	if (argc != 2)
	{
		write(1, "usage: ./fdf source_file\n", 25);
		exit(1);
	}
	else
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
		{
			write(1, "ERROR\n", 6);
			exit(1);
		}
		map = read_map(fd);
		close(fd);
		if (map == NULL)
			exit(1);
		else
			fdf(map);
	}
}
