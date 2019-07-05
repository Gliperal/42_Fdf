/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_map_to_screen.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 19:53:55 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/05 13:18:12 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rendering/rendering.h"
#include "param.h"
#include "map.h"

static int		color_scale(int color, float scale)
{
	int alpha;
	int red;
	int green;
	int blue;

	alpha = (color >> 24) & 0xFF;
	red = (color >> 16) & 0xFF;
	green = (color >> 8) & 0xFF;
	blue = color & 0xFF;
	red *= scale;
	green *= scale;
	blue *= scale;
	red &= 255;
	green &= 255;
	blue &= 255;
	return (alpha << 24) | (red << 16) | (green << 8) | blue;
}

static t_vertex	*clip_to_screen(t_vertex *clip, int sx, int sy, int fog)
{
	float		ndc_x;
	float		ndc_y;
	float		ndc_z;
	t_vertex	*v;

	if (clip == NULL)
		return (NULL);
	ndc_x = clip->x / clip->w;
	ndc_y = clip->y / clip->w;
	ndc_z = clip->z / clip->w;
	if (ndc_x < -1.5 || ndc_x > 1.5 || ndc_y < -1.5 || \
			ndc_y > 1.5 || ndc_z < -1 || ndc_z > 1)
		return (NULL);
	v = vertex_new(\
			(ndc_x + 1) * sx / 2, \
			(ndc_y + 1) * sy / 2, \
			0, 0);
	if (v == NULL)
		return (NULL);
	if (fog && ndc_z > 0.90)
		v->color = color_scale(clip->color, 10 - 10 * ndc_z);
	else
		v->color = clip->color;
	return (v);
}

static t_vertex	**transform_map_row_to_screen(t_param *param, int y)
{
	t_vertex	**new_row;
	int			x;
	t_vertex	*tmp;

	new_row = (t_vertex **)malloc(param->world->width * sizeof(t_vertex *));
	if (new_row == NULL)
		return (NULL);
	x = 0;
	while (x < param->world->width)
	{
		tmp = camera_vertex_to_clip(param->camera, param->world->data[y][x]);
		new_row[x] = clip_to_screen(tmp, \
				param->screen->width, param->screen->height, param->fog);
		free(tmp);
		x++;
	}
	return (new_row);
}

t_map			*transform_map_to_screen(t_param *param)
{
	t_map	*new;
	int		y;

	new = (t_map *)malloc(sizeof(t_map));
	if (!new)
		return (NULL);
	new->width = param->world->width;
	new->height = param->world->height;
	new->data = (t_vertex ***)malloc(new->height * sizeof(t_vertex **));
	if (!new->data)
		return (NULL);
	y = 0;
	while (y < new->height)
	{
		new->data[y] = transform_map_row_to_screen(param, y);
		if (new->data[y] == NULL)
			return (NULL);
		y++;
	}
	return (new);
}
