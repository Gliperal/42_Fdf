/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 20:36:57 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/05 13:06:08 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>

#include "input.h"
#include "param.h"
#include "minilibx_macos/mlx.h"
#include "misc.h"

const t_vertex	g_vertex_left = {-1, 0, 0, 0, 0};
const t_vertex	g_vertex_right = {1, 0, 0, 0, 0};
const t_vertex	g_vertex_up = {0, -1, 0, 0, 0};
const t_vertex	g_vertex_down = {0, 1, 0, 0, 0};
const t_vertex	g_vertex_back = {0, 0, -1, 0, 0};
const t_vertex	g_vertex_front = {0, 0, 1, 0, 0};
const t_point	g_rot_left = {-1, 0};
const t_point	g_rot_right = {1, 0};
const t_point	g_rot_up = {0, 1};
const t_point	g_rot_down = {0, -1};

static void		on_update1(t_param *param, float mspeed, float rspeed)
{
	if (key_down(param->input, ESC))
		exit(0);
	if (key_down(param->input, KEY_Q))
		camera_spin(param->camera, 0 - rspeed);
	if (key_down(param->input, KEY_E))
		camera_spin(param->camera, rspeed);
	if (key_down(param->input, KEY_A))
		camera_move(param->camera, &g_vertex_left, mspeed);
	if (key_down(param->input, KEY_D))
		camera_move(param->camera, &g_vertex_right, mspeed);
	if (key_down(param->input, KEY_W))
		camera_move(param->camera, &g_vertex_front, mspeed);
	if (key_down(param->input, KEY_S))
		camera_move(param->camera, &g_vertex_back, mspeed);
	if (key_down(param->input, ARROW_LEFT))
		camera_rotate(param->camera, g_rot_left, rspeed);
	if (key_down(param->input, ARROW_RIGHT))
		camera_rotate(param->camera, g_rot_right, rspeed);
	if (key_down(param->input, ARROW_UP))
		camera_rotate(param->camera, g_rot_up, rspeed);
	if (key_down(param->input, ARROW_DOWN))
		camera_rotate(param->camera, g_rot_down, rspeed);
	if (button_down(param->input, LCLICK))
		camera_rotate_screen(param->camera, param->input->mouse_moved, \
				param->screen);
}

static void		on_update2(t_param *param)
{
	if (key_down(param->input, MINUS))
	{
		param->world->z_scale *= 0.95;
		param->camera->pre_transformation->z->z = param->world->z_scale;
		param->camera->updated = 1;
	}
	if (key_down(param->input, PLUS))
	{
		param->world->z_scale *= 1.05;
		param->camera->pre_transformation->z->z = param->world->z_scale;
		param->camera->updated = 1;
	}
	if (param->input->key_states[KEY_F] == PRESSED)
	{
		param->fog ^= 1;
		param->camera->updated = 1;
	}
	if (param->input->exposed)
	{
		param->camera->updated = 1;
		param->input->exposed = 0;
	}
}

static void		on_update(void *p)
{
	t_param *param;
	float	mspeed;
	float	rspeed;

	param = (t_param *)p;
	mspeed = key_down(param->input, SHIFT) ? 0.42 : 0.042;
	rspeed = key_down(param->input, SHIFT) ? 2 : 0.42;
	on_update1(param, mspeed, rspeed);
	on_update2(param);
	render(param);
}

static void		fdf(t_map *map)
{
	t_param		*param;
	t_screen	*screen;
	float		aspect_ratio;
	float		far;

	param = (t_param *)malloc(sizeof(t_param));
	if (param == NULL)
		do_exit("Param creation failed. Exiting...", 1);
	screen = new_screen(1280, 720, "Hello world!");
	far = ft_max(ft_max(map->width, map->height), map->max - map->min) * 2;
	far = ft_max(42, far);
	aspect_ratio = (float)screen->width / screen->height;
	param->camera = camera_new(60, 1, far, aspect_ratio);
	param->input = input_new(&on_update, param, screen);
	if (screen == NULL || param->camera == NULL || param->input == NULL)
		do_exit("Things creation failed. Exiting...", 1);
	param->camera->position->x = map->width / 2;
	param->camera->position->y = map->height / 2;
	param->camera->position->z = map->min - 10;
	param->screen = screen;
	param->world = map;
	param->fog = 1;
	mlx_loop(screen->mlx_ptr);
}

int				main(int argc, char **argv)
{
	int		fd;
	t_map	*map;

	if (argc != 2)
		do_exit("usage: ./fdf source_file", 1);
	else
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
		{
			write(1, "File load error.", 1);
			exit(1);
		}
		map = map_read(fd);
		close(fd);
		if (map == NULL)
			do_exit("Bad map.", 1);
		else
			fdf(map);
	}
}
