/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 20:36:57 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/04 13:07:59 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "map.h"
#include "libft/libft.h"
#include "rendering/rendering.h"

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
		t_vertex *v = vertex_new(row_size, map->height, 0 - ft_pop_atoi(line + i), 1);
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

const unsigned char g_min_color[3] = {0xFF, 0xFF, 0xFF};
const unsigned char g_mid_color[3] = {0x8B, 0x45, 0x13};
const unsigned char g_max_color[3] = {0x22, 0x8B, 0x22};

void	color_map(t_map *map)
{
	float min;
	float mid;
	float max;
	unsigned int color[3];

	min = 2147483647;
	max = -2147483648;
	for (int y = 0; y < map->height; y++)
		for (int x = 0; x < map->width; x++)
		{
			if (map->data[y][x]->z < min)
				min = map->data[y][x]->z;
			if (map->data[y][x]->z > max)
				max = map->data[y][x]->z;
		}
	mid = (min + max) / 2;
	if (min == mid)
		min -= 1;
	if (max == mid)
		max += 1;
	for (int y = 0; y < map->height; y++)
		for (int x = 0; x < map->width; x++)
		{
			if (map->data[y][x]->z < mid)
			{
				float dist = (map->data[y][x]->z - min) / (mid - min);
				color[0] = dist * g_mid_color[0] + (1 - dist) * g_min_color[0];
				color[1] = dist * g_mid_color[1] + (1 - dist) * g_min_color[1];
				color[2] = dist * g_mid_color[2] + (1 - dist) * g_min_color[2];
				map->data[y][x]->color = (color[0] << 16) + (color[1] << 8) + color[2];
			}
			else
			{
				float dist = (map->data[y][x]->z - mid) / (max - mid);
				color[0] = dist * g_max_color[0] + (1 - dist) * g_mid_color[0];
				color[1] = dist * g_max_color[1] + (1 - dist) * g_mid_color[1];
				color[2] = dist * g_max_color[2] + (1 - dist) * g_mid_color[2];
				map->data[y][x]->color = (color[0] << 16) + (color[1] << 8) + color[2];
			}
		}
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
		{
			color_map(map);
			return (map);
		}
	}
}
