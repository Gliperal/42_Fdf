/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 20:36:57 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/05 12:52:27 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "map.h"
#include "libft/libft.h"

/*
** I'm being naughty and not freeing memory on every malloc failure, but don't
** tell anyone, okay?
*/

static int	read_map_row_values(t_vertex ***row, char *line, int r_size, int y)
{
	int			i;
	t_vertex	**tmp;
	t_vertex	*v;

	*row = NULL;
	i = 0;
	while (1)
	{
		while (ft_iswhite(line[i]))
			i++;
		if (line[i] == 0)
			return (r_size);
		if (line[i] != '+' && line[i] != '-' && !ft_isdigit(line[i]))
			return (0);
		tmp = *row;
		*row = malloc((r_size + 1) * sizeof(t_vertex *));
		if (*row == NULL)
			return (0);
		ft_memcpy(*row, tmp, r_size * sizeof(t_vertex *));
		free(tmp);
		v = vertex_new(r_size, y, 0 - ft_pop_atoi(line + i), 1);
		(*row)[r_size] = v;
		r_size++;
	}
}

static int	read_map_row(int fd, t_map *map)
{
	char		*line;
	int			status;
	t_vertex	**row;
	t_vertex	***tmp2;
	int			row_size;

	status = get_next_line(fd, &line);
	if (status != 1)
		return (status);
	row_size = read_map_row_values(&row, line, 0, map->height);
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

t_map		*map_read(int fd)
{
	t_map	*map;
	int		status;

	map = (t_map *)malloc(sizeof(t_map));
	if (map == NULL)
		return (NULL);
	map->width = 0;
	map->height = 0;
	map->data = NULL;
	map->z_scale = 1;
	while (1)
	{
		status = read_map_row(fd, map);
		if (status == -1)
			return (NULL);
		else if (status == 0)
		{
			map_color(map);
			return (map);
		}
	}
}

void		map_free(t_map *map)
{
	int y;
	int x;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			free(map->data[y][x]);
			x++;
		}
		free(map->data[y]);
		y++;
	}
	free(map->data);
	free(map);
}
