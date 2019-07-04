/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 19:53:55 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/03 16:56:10 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "mlx.h"
#include "rendering/mlx_util.h"
#include "rendering/rendering.h"
#include "param.h"
#include "libft/libft.h"
#include "map.h"

void	free_map(t_map *map)
{
	for (int y = 0; y < map->height; y++)
	{
		for (int x = 0; x < map->width; x++)
			free(map->data[y][x]);
		free(map->data[y]);
	}
	free(map->data);
	free(map);
}

void	vertex_print(t_vertex *v)
{
	ft_printf("(%.2f, %.2f, %.2f, %.2f)", v->x, v->y, v->z, v->w);
}

# define SCREEN_WIDTH 1280
# define SCREEN_HEIGHT 720

t_map	*transform_map_to_ndc(t_map *map, t_camera *camera)
{
	t_map *new = (t_map *)malloc(sizeof(t_map));
	if (!new)
		return (NULL);
	new->width = map->width;
	new->height = map->height;
	new->data = (t_vertex ***)malloc(new->height * sizeof(t_vertex **));
	if (!new->data)
		return (NULL);
	for (int y = 0; y < new->height; y++)
	{
		new->data[y] = (t_vertex **)malloc(new->width * sizeof(t_vertex *));
		if (!new->data[y])
			return (NULL);
		for (int x = 0; x < new->width; x++)
		{
			new->data[y][x] = camera_vertex_to_clip(camera, map->data[y][x]);
			// perform clipping
			if (!new->data[y][x])
				return (NULL);
//			vertex_print(map->data[y][x]);
//			ft_printf(" -> ");
//			vertex_print(new->data[y][x]);
//			ft_printf("\n");
			// clip coordinates to NDC coordinates
			new->data[y][x]->x = new->data[y][x]->x / new->data[y][x]->w;
			new->data[y][x]->y = new->data[y][x]->y / new->data[y][x]->w;
			new->data[y][x]->z = new->data[y][x]->z / new->data[y][x]->w;
			if (new->data[y][x]->z < -1 || new->data[y][x]->z > 1)
			{
				free(new->data[y][x]);
				new->data[y][x] = NULL;
			}
			else
			{
				// NDC coordinates to screen coordinates
				float half_width = SCREEN_WIDTH / 2;
				float half_height = SCREEN_HEIGHT / 2;
				new->data[y][x]->x = new->data[y][x]->x * half_width + half_width;
				new->data[y][x]->y = new->data[y][x]->y * half_height + half_height;
			}
		}
	}
	return (new);
}

void	put_map_to_screen(t_map *map, t_screen *screen)
{
	t_vertex *src;
	t_vertex *dst;
	t_point psrc;
	t_point pdst;

	ft_bzero(screen->data, screen->width * screen->height * screen->bpp / 8);
	for (int y = 0; y < map->height; y++)
		for (int x = 0; x < map->width; x++)
		{
			src = map->data[y][x];
			if (src == NULL)
				continue;
			psrc.x = (int) src->x;
			psrc.y = (int) src->y;
			if (y > 0)
			{
				dst = map->data[y - 1][x];
				if (dst == NULL)
					continue;
				pdst.x = (int) dst->x;
				pdst.y = (int) dst->y;
				ft_draw_line(screen, psrc, pdst, 0x0088CCFF);
			}
			if (x > 0)
			{
				dst = map->data[y][x - 1];
				if (dst == NULL)
					continue;
				pdst.x = (int) dst->x;
				pdst.y = (int) dst->y;
				ft_draw_line(screen, psrc, pdst, 0x00FFCC88);
			}
		}
}

void	redraw(t_param *param)
{
	t_map *transformed = transform_map_to_ndc(param->world, param->camera);
	put_map_to_screen(transformed, param->screen);
	free_map(transformed);
}

void	render(t_param *param)
{
	t_screen *screen;

	screen = param->screen;
	if (param->camera->updated)
		redraw(param);
	mlx_put_image_to_window(screen->mlx_ptr, screen->win_ptr, screen->img_ptr, 0, 0);
	t_vertex *pos = param->camera->position;
	t_quat *rot = param->camera->rotation;
	char *str = ft_strprintf("Camera: %+.2f, %+.2f, %+.2f (%+.2f + %+.2fi + %+.2fj + %+.2fk)", pos->x, pos->y, pos->z, rot->s, rot->i, rot->j, rot->k);
	mlx_string_put(screen->mlx_ptr, screen->win_ptr, 0, 0, 0xFFFFFF, str);
	free(str);
}

/*
** No mlx_destroy_ptr. Memory leaks abound :(
*/

t_screen	*new_screen(int width, int height, char *title)
{
	t_screen	*screen;

	screen = (t_screen *)malloc(sizeof(t_screen));
	if (screen == NULL)
		return (NULL);
	screen->mlx_ptr = mlx_init();
	if (screen->mlx_ptr == NULL)
	{
		free(screen);
		return (NULL);
	}
	screen->win_ptr = mlx_new_window(screen->mlx_ptr, width, height, title);
	if (screen->win_ptr == NULL)
		return (NULL);
	screen->img_ptr = mlx_new_image(screen->mlx_ptr, width, height);
	if (screen->img_ptr == NULL)
		return (NULL);
	screen->data = (void *)mlx_get_data_addr(screen->img_ptr, &screen->bpp, &screen->size_line, &screen->endian);
	if (screen->data == NULL)
		return (NULL);
	screen->width = width;
	screen->height = height;
	return (screen);
}

void	do_exit(char *err, int exit_code)
{
	ft_putendl(err);
	exit(exit_code);
}

void	on_update(void *p)
{
	t_param *param;

	param = (t_param *)p;
	if (param->input->button_states[ESC] == RELEASED)
		exit(0);
	if (param->input->button_states[LCLICK] == HELD)
		camera_rotate(param->camera, param->input->mouse_moved);
	if (param->input->button_states[KEY_E] == PRESSED)
		camera_spin(param->camera, 1);
	if (param->input->button_states[KEY_Q] == PRESSED)
		camera_spin(param->camera, -1);
	if (param->input->button_states[RCLICK] == HELD)
		ft_printf("Right mouse dragged %d,%d\n", param->input->mouse_moved.x, param->input->mouse_moved.y);
	render(param);
}

void	fdf(t_map *map)
{
	t_param *param = (t_param *)malloc(sizeof(t_param));
	if (param == NULL)
		do_exit("Param creation failed. Exiting...", 1);
	t_screen *screen = new_screen(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello world!");
	float far = ft_max(42, ft_max(map->width, map->height) * 3);
	param->camera = camera_new(60, 1, far, (float) screen->width / screen->height);
	param->input = input_new(&on_update, param, screen->win_ptr);
	if (screen == NULL || param->camera == NULL || param->input == NULL)
		do_exit("Things creation failed. Exiting...", 1);
	param->camera->position->x = map->width / 2;
	param->camera->position->y = map->height / 2;
	param->screen = screen;
	param->world = map;
	render(param);
	mlx_do_key_autorepeatoff(screen->mlx_ptr);
	mlx_loop(screen->mlx_ptr);
}
