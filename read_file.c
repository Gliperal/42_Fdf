/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 20:36:57 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/05 00:03:58 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "map.h"
#include "libft/libft.h"
#include "rendering/rendering.h"

/*
** I'm being naughty and not freeing memory on every malloc failure, but don't
** tell anyone, okay?
*/

static int	read_map_row_values(t_vertex ***row, char *line, int row_size, int y)
{
	int i;
	t_vertex **tmp;
	t_vertex *v;

	*row = NULL;
	i = 0;
	while (1)
	{
		while (ft_iswhite(line[i]))
			i++;
		if (line[i] == 0)
			return (row_size);
		if (line[i] != '+' && line[i] != '-' && !ft_isdigit(line[i]))
			return (0);
		tmp = *row;
		*row = malloc((row_size + 1) * sizeof(t_vertex *));
		if (*row == NULL)
			return (0);
		ft_memcpy(*row, tmp, row_size * sizeof(t_vertex *));
		free(tmp);
		v = vertex_new(row_size, y, 0 - ft_pop_atoi(line + i), 1);
		(*row)[row_size] = v;
		row_size++;
	}
}

static int	read_map_row(int fd, t_map *map)
{
	char *line;
	int status;
	t_vertex **row;
	t_vertex ***tmp2;
	int row_size;

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

static void	map_get_min_max(t_map *map)
{
	int y;
	int x;

	map->min = 2147483647;
	map->max = -2147483648;
	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			if (map->data[y][x]->z < map->min)
				map->min = map->data[y][x]->z;
			if (map->data[y][x]->z > map->max)
				map->max = map->data[y][x]->z;
			x++;
		}
		y++;
	}
}

const unsigned char g_min_color[3] = {0xFF, 0xFF, 0xFF};
const unsigned char g_mid_color[3] = {0x8B, 0x45, 0x13};
const unsigned char g_max_color[3] = {0x22, 0x8B, 0x22};

static void	color_vertex(t_vertex *v, float min, float mid, float max)
{
	float			dist;
	unsigned int	color[3];

	if (v->z < mid)
	{
		dist = (v->z - min) / (mid - min);
		color[0] = dist * g_mid_color[0] + (1 - dist) * g_min_color[0];
		color[1] = dist * g_mid_color[1] + (1 - dist) * g_min_color[1];
		color[2] = dist * g_mid_color[2] + (1 - dist) * g_min_color[2];
		v->color = (color[0] << 16) + (color[1] << 8) + color[2];
	}
	else
	{
		dist = (v->z - mid) / (max - mid);
		color[0] = dist * g_max_color[0] + (1 - dist) * g_mid_color[0];
		color[1] = dist * g_max_color[1] + (1 - dist) * g_mid_color[1];
		color[2] = dist * g_max_color[2] + (1 - dist) * g_mid_color[2];
		v->color = (color[0] << 16) + (color[1] << 8) + color[2];
	}
}

static void	color_map(t_map *map)
{
	float	mid;
	int		y;
	int		x;

	map_get_min_max(map);
	mid = (map->min + map->max) / 2;
	if (map->min == mid)
		map->min -= 1;
	if (map->max == mid)
		map->max += 1;
	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			color_vertex(map->data[y][x], map->min, mid, map->max);
			x++;
		}
		y++;
	}
}

t_map	*read_map(int fd)
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
			color_map(map);
			return (map);
		}
	}
}
