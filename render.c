/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 19:53:55 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/05 12:54:06 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minilibx_macos/mlx.h"
#include "param.h"
#include "libft/libft.h"
#include "map.h"
#include "transform_map_to_screen.h"

t_cpoint	vertex_to_cpoint(t_vertex *vertex)
{
	t_cpoint cpoint;

	cpoint.x = (int)vertex->x;
	cpoint.y = (int)vertex->y;
	cpoint.color = vertex->color;
	return (cpoint);
}

static void	put_edges_to_screen(t_map *map, t_screen *screen, int x, int y)
{
	t_vertex *src;
	t_vertex *dst;
	t_cpoint psrc;
	t_cpoint pdst;

	src = map->data[y][x];
	if (src == NULL)
		return ;
	psrc = vertex_to_cpoint(src);
	if (y > 0)
	{
		dst = map->data[y - 1][x];
		if (dst == NULL)
			return ;
		pdst = vertex_to_cpoint(dst);
		ft_draw_line(screen, psrc, pdst);
	}
	if (x > 0)
	{
		dst = map->data[y][x - 1];
		if (dst == NULL)
			return ;
		pdst = vertex_to_cpoint(dst);
		ft_draw_line(screen, psrc, pdst);
	}
}

static void	redraw(t_param *param)
{
	t_map		*transformed;
	int			x;
	int			y;

	transformed = transform_map_to_screen(param);
	ft_bzero(param->screen->data, param->screen->width * \
			param->screen->height * param->screen->bpp / 8);
	y = 0;
	while (y < transformed->height)
	{
		x = 0;
		while (x < transformed->width)
		{
			put_edges_to_screen(transformed, param->screen, x, y);
			x++;
		}
		y++;
	}
	map_free(transformed);
}

void		render(t_param *param)
{
	t_screen	*screen;
	t_vertex	*pos;
	t_quat		*rot;
	char		*cam_str;
	char		*fps_str;

	if (param->camera->updated)
		redraw(param);
	screen = param->screen;
	mlx_put_image_to_window(screen->mlx_ptr, screen->win_ptr, screen->img_ptr,\
			0, 0);
	pos = param->camera->position;
	rot = param->camera->rotation;
	cam_str = ft_strprintf("Camera: %+.2f, %+.2f, %+.2f \
			(%+.2f + %+.2fi + %+.2fj + %+.2fk)", \
			pos->x, pos->y, pos->z, rot->s, rot->i, rot->j, rot->k);
	fps_str = ft_strprintf("FPS: %5.2f\n", param->input->fps);
	mlx_string_put(screen->mlx_ptr, screen->win_ptr, 0, 0, 0xFFFFFF, cam_str);
	mlx_string_put(screen->mlx_ptr, screen->win_ptr, 0, 20, 0xFFFFFF, fps_str);
	free(cam_str);
	free(fps_str);
}
