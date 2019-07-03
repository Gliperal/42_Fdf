/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 19:53:55 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/02 18:47:01 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "mlx.h"
#include "mlx_util.h"
#include "param.h"
#include "quaternion.h"
#include "libft/libft.h"

#include "vertex.h"
#include "matrix.h"
#include "map.h"

void	ft_draw_line(t_screen *screen, t_point src, t_point dst, int color);

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

void	test_print(t_vertex *old, t_vertex *new)
{
	ft_printf("\t(%.2f, %.2f, %.2f, %.2f) -> (%.2f, %.2f, %.2f, %.2f)\n", old->x, old->y, old->z, old->w, new->x, new->y, new->z, new->w);
}

# define SCREEN_WIDTH 640
# define SCREEN_HEIGHT 480

t_map	*transform_map_to_ndc(t_matrix *matrix, t_map *map)
{
	ft_printf("TRANSFORM MAP\n");
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
			new->data[y][x] = transform_vertex(matrix, map->data[y][x]);
			if (!new->data[y][x])
				return (NULL);
			// TODO perform clipping
			test_print(map->data[y][x], new->data[y][x]);
			// clip coordinates to NDC coordinates
			new->data[y][x]->x = new->data[y][x]->x / new->data[y][x]->w;
			new->data[y][x]->y = new->data[y][x]->y / new->data[y][x]->w;
			new->data[y][x]->z = new->data[y][x]->z / new->data[y][x]->w;
			// NDC coordinates to screen coordinates
			float half_width = SCREEN_WIDTH / 2;
			float half_height = SCREEN_HEIGHT / 2;
			new->data[y][x]->x = new->data[y][x]->x * half_width + half_width;
			new->data[y][x]->y = new->data[y][x]->y * half_height + half_height;
			test_print(map->data[y][x], new->data[y][x]);
		}
	}
	return (new);
}

void	test_print_matrix(t_matrix *matrix)
{
	t_vertex *v1 = matrix->x;
	t_vertex *v2 = matrix->y;
	t_vertex *v3 = matrix->z;
	t_vertex *v4 = matrix->w;
	ft_printf("[%7.2f %7.2f %7.2f %7.2f]\n[%7.2f %7.2f %7.2f %7.2f]\n[%7.2f %7.2f %7.2f %7.2f]\n[%7.2f %7.2f %7.2f %7.2f]\n",
			v1->x, v2->x, v3->x, v4->x,
			v1->y, v2->y, v3->y, v4->y,
			v1->z, v2->z, v3->z, v4->z,
			v1->w, v2->w, v3->w, v4->w
	);
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
			psrc.x = (int) src->x;
			psrc.y = (int) src->y;
			if (y > 0)
			{
				dst = map->data[y - 1][x];
				pdst.x = (int) dst->x;
				pdst.y = (int) dst->y;
				ft_draw_line(screen, psrc, pdst, 0x0088CCFF);
			}
			if (x > 0)
			{
				dst = map->data[y][x - 1];
				pdst.x = (int) dst->x;
				pdst.y = (int) dst->y;
				ft_draw_line(screen, psrc, pdst, 0x00FFCC88);
			}
		}
}

#include <math.h>
void	redraw(t_param *param)
{
	t_matrix *m_translate = matrix_new(
		vertex_new(1, 0, 0, 0),
		vertex_new(0, 1, 0, 0),
		vertex_new(0, 0, 1, 0),
		vertex_new(0, 0, 10, 1)
	);
	float s = param->camera->rotation->s;
	float i = param->camera->rotation->i;
	float j = param->camera->rotation->j;
	float k = param->camera->rotation->k;
	// Quaternion rotation to matrix rotation
	t_matrix *m_rotate = matrix_new(
		vertex_new(1 - 2 * (j * j + k * k), 2 * (i * j + k * s), 2 * (i * k - j * s), 0),
		vertex_new(2 * (i * j - k * s), 1 - 2 * (i * i + k * k), 2 * (j * k + i * s), 0),
		vertex_new(2 * (i * k + j * s), 2 * (j * k - i * s), 1 - 2 * (i * i + j * j), 0),
		vertex_new(0, 0, 0, 1)
	);
	float fov = 60;
	ft_printf("fov = %f\n", fov);
	float n = 1;
	ft_printf("n = %f\n", n);
	float f = 42;
	ft_printf("f = %f\n", f);
	float scale = tan(fov * 0.5 * M_PI / 180) * n;
	ft_printf("scale = %f\n", scale);
	float ar = (float) SCREEN_WIDTH / SCREEN_HEIGHT;
	ft_printf("ar = %f\n", ar);
	float r = ar * scale;
	ft_printf("r = %f\n", r);
	float l = 0 - r;
	ft_printf("l = %f\n", l);
	float b = scale;
	ft_printf("b = %f\n", b);
	float t = 0 - b;
	ft_printf("t = %f\n", t);
	float m1 = 2 * n / (r - l);
	float m2 = 2 * n / (b - t);
	float m3 = (r + l) / (r - l);
	float m4 = (b + t) / (b - t);
	float m5 = (f + n) / (f - n);
	float m6 = 0 - 2 * f * n / (f - n);
	t_matrix *m_proj = matrix_new(
		vertex_new(m1, 0, 0, 0),
		vertex_new(0, m2, 0, 0),
		vertex_new(m3, m4, m5, 1),
		vertex_new(0, 0, m6, 0)
	);
	test_print_matrix(m_proj);
	t_matrix *m_trtt = matrix_multiply(m_rotate, m_translate);
	t_matrix *m_prtrtt = matrix_multiply(m_proj, m_trtt);
	// TODO free things (matrices, vertices, etc.)
	t_map *transformed = transform_map_to_ndc(m_prtrtt, param->world);
	put_map_to_screen(transformed, param->screen);
	free_map(transformed);
}

void	render(t_param *param)
{
	t_screen *screen;

	screen = param->screen;
	if (param->camera_updated)
	{
		param->camera_updated = 0;
		redraw(param);
	}
	mlx_put_image_to_window(screen->mlx_ptr, screen->win_ptr, screen->img_ptr, 0, 0);
	t_quat *rot = param->camera->rotation;
	char *str = ft_strprintf("Camera: 0, 0, 0 (%+.2f + %+.2fi + %+.2fj + %+.2fk)", rot->s, rot->i, rot->j, rot->k);
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
	{
		param->camera_updated = 1;
		camera_rotate(param->camera, x - param->mouse_x, y - param->mouse_y);
	}
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
	param->camera_updated = 1;
	t_screen *screen = new_screen(640, 480, "Hello world!");
	param->camera = (t_camera *)malloc(sizeof(t_camera));
	if (screen == NULL || param->camera == NULL)
		do_exit("Things creation failed. Exiting...", 1);
	param->camera->x = 0;
	param->camera->y = 0;
	param->camera->z = 0;
	param->camera->rotation = quaternion_new(1, 0, 0, 0);
	if (param->camera->rotation == NULL)
		do_exit("Camera rotation creation failed. Exiting...", 1);
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
