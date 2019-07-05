/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 19:53:55 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/04 18:12:21 by nwhitlow         ###   ########.fr       */
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

void	vertex_print(t_vertex *v)
{
	ft_printf("(%.2f, %.2f, %.2f, %.2f)", v->x, v->y, v->z, v->w);
}

void	free_map(t_map *map)
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

void	color_set_alpha(int *color, int alpha)
{
	*color = *color & 0xFFFFFF;
	*color = *color | (alpha << 24);
}

int	color_scale(int color, float scale)
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

t_vertex	*clip_vertex_to_screen(t_vertex *clip, int s_width, int s_height)
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
	if (ndc_x < -1.5 || ndc_x > 1.5 || ndc_y < -1.5 || ndc_y > 1.5 || ndc_z < -1 || ndc_z > 1)
		return (NULL);
	v = vertex_new(\
			(ndc_x + 1) * s_width / 2, \
			(ndc_y + 1) * s_height / 2, \
			0, 0);
	if (v == NULL)
		return (NULL);
//	if (ndc_z > 0.90)
//		v->color = color_scale(clip->color, 10 - 10 * ndc_z);
//	else
		v->color = clip->color;
	return (v);
}

t_map	*transform_map_to_screen(t_param *param)
{
	t_map		*new;
	int			x;
	int			y;
	t_vertex	*old;

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
		new->data[y] = (t_vertex **)malloc(new->width * sizeof(t_vertex *));
		if (!new->data[y])
			return (NULL);
		x = 0;
		while (x < new->width)
		{
			new->data[y][x] = camera_vertex_to_clip(param->camera, \
					param->world->data[y][x]);
			old = new->data[y][x];
			new->data[y][x] = clip_vertex_to_screen(new->data[y][x], \
					param->screen->width, param->screen->height);
			free(old);
			x++;
		}
		y++;
	}
	return (new);
}

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

void	put_map_to_screen(t_map *map, t_screen *screen)
{
	int x;
	int y;

	ft_bzero(screen->data, screen->width * screen->height * screen->bpp / 8);
	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			put_edges_to_screen(map, screen, x, y);
			x++;
		}
		y++;
	}
}

void	redraw(t_param *param)
{
	t_map	*transformed;
	t_point	screen_size;

	screen_size.x = param->screen->width;
	screen_size.y = param->screen->height;
	transformed = transform_map_to_screen(param);
	put_map_to_screen(transformed, param->screen); // TODO the whole function can fit here
	free_map(transformed);
}

void	render(t_param *param)
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
	screen->data = (void *)mlx_get_data_addr(screen->img_ptr, &screen->bpp, \
			&screen->size_line, &screen->endian);
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

static int	key_down(t_input *input, int button)
{
	return (input->key_states[button] == HELD ||
			input->key_states[button] == PRESSED);
}

static int	button_down(t_input *input, int button)
{
	return (input->button_states[button] == HELD ||
			input->button_states[button] == PRESSED);
}

static void	on_update(void *p)
{
	t_param *param;
	float	mspeed;
	float	rspeed;

	param = (t_param *)p;
	mspeed = key_down(param->input, SHIFT) ? 0.42 : 0.042;
	rspeed = key_down(param->input, SHIFT) ? 2 : 0.42;
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
		camera_rotate_screen(param->camera, param->input->mouse_moved, param->screen);
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
	if (param->input->exposed)
		param->camera->updated = 1;
	render(param);
}

void	fdf(t_map *map)
{
	t_param		*param;
	t_screen	*screen;
	float		aspect_ratio;
	float		far;

	param = (t_param *)malloc(sizeof(t_param));
	if (param == NULL)
		do_exit("Param creation failed. Exiting...", 1);
	screen = new_screen(1280, 720, "Hello world!");
	far = ft_max(42, ft_max(ft_max(map->width, map->height), map->max - map->min) * 2);
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
	mlx_loop(screen->mlx_ptr);
}
