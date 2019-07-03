/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 19:53:55 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/03 13:23:16 by nwhitlow         ###   ########.fr       */
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
//			test_print(map->data[y][x], new->data[y][x]);
			if (!new->data[y][x])
				return (NULL);
			// TODO perform clipping
//			test_print(map->data[y][x], new->data[y][x]);
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
//				test_print(map->data[y][x], new->data[y][x]);
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

	// Clear image
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

int	handle_mouse_press(int button, int x, int y, t_param *param)
{
	if (button == LCLICK)
		param->mouse1held = 1;
	if (button == RCLICK)
		param->mouse2held = 1;
	param->mouse_x = x;
	param->mouse_y = y;
	return (0);
}

int	handle_mouse_release(int button, int x, int y, t_param *param)
{
	if (button == LCLICK)
		param->mouse1held = 0;
	if (button == RCLICK)
		param->mouse2held = 0;
	param->mouse_x = x;
	param->mouse_y = y;
	return (0);
}

#include <math.h>

void	camera_rotate(t_camera *camera, int x, int y)
{
	t_quat rot;
	float angx;
	float angy;
	float cosx;
	float sinx;
	float cosy;
	float siny;

	camera->updated = 1;
	angx = (float) x / 10 * (M_PI / 180);
	angy = (float) (0 - y) / 10 * (M_PI / 180);
	cosx = cos(angx);
	sinx = sin(angx);
	cosy = cos(angy);
	siny = sin(angy);
	rot.s = cosx * cosy;
	rot.i = cosx * siny;
	rot.j = sinx * cosy;
	rot.k = sinx * siny;
	quaternion_left_multiply(camera->rotation, &rot);
}

int handle_mouse_move(int x, int y, t_param *param)
{
	if (param->mouse1held && (x != param->mouse_x || y != param->mouse_y))
		camera_rotate(param->camera, x - param->mouse_x, y - param->mouse_y);
	if (param->mouse2held)
		ft_printf("Right mouse dragged %d,%d\n", x - param->mouse_x, y - param->mouse_y);
	param->mouse_x = x;
	param->mouse_y = y;
	render(param);
	return (0);
}

int	handle_key_press(int key, void *param)
{
	key = 0;
	param = 0;
	return 0;
}

int handle_key_release(int key, void *param)
{
	param = 0;
	if (key == ESC)
		exit (0);
	return 0;
}

int	handle_exit()
{
	exit (0);
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

void	fdf(t_map *map)
{
	t_param *param = (t_param *)malloc(sizeof(t_param));
	if (param == NULL)
		do_exit("Param creation failed. Exiting...", 1);
	param->mouse1held = 0;
	param->mouse2held = 0;
	t_screen *screen = new_screen(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello world!");
	param->camera = camera_new(60, 1, 42, (float) SCREEN_WIDTH / SCREEN_HEIGHT);
	if (screen == NULL || param->camera == NULL)
		do_exit("Things creation failed. Exiting...", 1);
	param->screen = screen;
	param->world = map;
	render(param);
	mlx_do_key_autorepeatoff(screen->mlx_ptr);
	mlx_hook(screen->win_ptr, 2, 0, &handle_key_press, NULL);
	mlx_hook(screen->win_ptr, 3, 0, &handle_key_release, NULL);
	mlx_hook(screen->win_ptr, 4, 0, &handle_mouse_press, param);
	mlx_hook(screen->win_ptr, 5, 0, &handle_mouse_release, param);
	mlx_hook(screen->win_ptr, 6, 0, &handle_mouse_move, param);
	mlx_hook(screen->win_ptr, 17, 0, &handle_exit, NULL);
	mlx_loop(screen->mlx_ptr);
}
