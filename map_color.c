/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_color.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 20:36:57 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/05 12:09:26 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "map.h"

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

void		map_color(t_map *map)
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
