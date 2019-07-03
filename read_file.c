/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 20:36:57 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/02 21:27:01 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "map.h"
#include "libft/libft.h"
# include "rendering/rendering.h"

/*
static void	free_map(t_map *map)
{
	for (int i = 0; i < map->height; i++)
	{
		for (int j = 0; j < map->width; j++)
			free(map->data[i][j]);
		free(map->data[i]);
	}
	free(map->data);
	free(map);
}
*/

/*
** I'm being naughty and not freeing memory on every malloc failure, but don't
** tell anyone, okay?
*/

static int	read_map_row(int fd, t_map *map)
{
	char *line;
	int status;
	t_vertex **row;
	t_vertex **tmp;
	t_vertex ***tmp2;
	int row_size;
	int i;

	status = get_next_line(fd, &line);
	if (status != 1)
		return (status);
	i = 0;
	row = NULL;
	row_size = 0;
	while (1)
	{
		while (ft_iswhite(line[i]))
			i++;
		if (line[i] == 0)
			break;
		if (line[i] != '+' && line[i] != '-' && (line[i] < '0' || line[i] > '9'))
			return (-1);
		tmp = row;
		row = malloc((row_size + 1) * sizeof(t_vertex *));
		if (row == NULL)
			return (-1);
		ft_memcpy(row, tmp, row_size * sizeof(t_vertex *));
		free(tmp);
		t_vertex *v = vertex_new(row_size, map->height, ft_pop_atoi(line + i), 1);
		row[row_size] = v;
		row_size++;
	}
	if (row_size == 0)
		return (-1);
	if (map->height != 0 && row_size != map->width)
		return (-1);
	map->width = row_size;
	tmp2 = map->data;
	map->data = malloc((map->height + 1) * sizeof(t_vertex **));
	if (map->data == NULL)
		return (-1);
	ft_memcpy(map->data, tmp2, map->height * sizeof(t_vertex **));
	free(tmp2);
	map->data[map->height] = row;
	map->height += 1;
	free(line);
	return (1);
}

t_map	*read_map(int fd)
{
	t_map *map;
	int status;

	map = (t_map *)malloc(sizeof(t_map));
	if (map == NULL)
		return (NULL);
	map->width = 0;
	map->height = 0;
	map->data = NULL;
	while (1)
	{
		status = read_map_row(fd, map);
		if (status == -1)
			return (NULL);
		else if (status == 0)
			return (map);
	}
}
