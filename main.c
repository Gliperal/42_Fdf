/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 19:53:55 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/01 16:31:18 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "mlx.h"
#include "mlx_util.h"
#include "param.h"
#include "quaternion.h"

// TODO REMOVE
#include <stdio.h>
#include <string.h>

void	redraw(t_param *param)
{
	t_screen *screen = param->screen;
	for (int y = 0; y < screen->height; y++)
		for (int x = 0; x < screen->width; x++)
		{
			int offset = (screen->width * y + x) * screen->bpp / 8;
			screen->data[offset] = y % 256;
			screen->data[offset + 1] = y % 256;
			screen->data[offset + 2] = y % 256;
		}
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
	mlx_pixel_put(screen->mlx_ptr, screen->win_ptr, 300, 240, 0xFF00FF);
	t_quat *rot = param->camera->rotation;
	char *str = malloc(100);
	sprintf(str, "Camera: 0, 0, 0 (%+.2f + %+.2fi + %+.2fj + %+.2fk)", rot->s, rot->i, rot->j, rot->k);
	mlx_string_put(screen->mlx_ptr, screen->win_ptr, 0, 0, 0xFFFFFF, str);
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
	angy = (float) y / 10 * (M_PI / 180);
	cosx = cos(angx);
	sinx = sin(angx);
	cosy = cos(angy);
	siny = sin(angy);
	rot.s = cosx * cosy;
	rot.i = sinx * siny;
	rot.j = cosx * siny;
	rot.k = sinx * cosy;
	quaternion_right_multiply(camera->rotation, &rot);
}

int handle_mouse_move(int x, int y, t_param *param)
{
	if (param->mouse1held && (x != param->mouse_x || y != param->mouse_y))
	{
		param->camera_updated = 1;
		camera_rotate(param->camera, x - param->mouse_x, y - param->mouse_y);
	}
	if (param->mouse2held)
		printf("Right mouse dragged %d,%d\n", x - param->mouse_x, y - param->mouse_y);
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
//	screen->size_line = BPP * width;
//	screen->bpp = BPP;
//	screen->endian = MACOS_ENDIAN;
	screen->data = (void *)mlx_get_data_addr(screen->img_ptr, &screen->bpp, &screen->size_line, &screen->endian);
	if (screen->data == NULL)
		return (NULL);
	screen->width = width;
	screen->height = height;
	return (screen);
}

int main()
{
	t_param *param = (t_param *)malloc(sizeof(t_param));
	if (param == NULL)
	{
		write(1, "Param creation failed. Exiting...\n", 34);
		exit(1);
	}
	param->mouse1held = 0;
	param->mouse2held = 0;
	param->camera_updated = 1;
	t_screen *screen = new_screen(640, 480, "Hello world!");
	param->world = (t_world *)malloc(sizeof(t_world));
	param->camera = (t_camera *)malloc(sizeof(t_camera));
	if (screen == NULL || param->world == NULL || param->camera == NULL)
	{
		write(1, "Things creation failed. Exiting...\n", 35);
		exit(1);
	}
	param->camera->x = 0;
	param->camera->y = 0;
	param->camera->z = 0;
	param->camera->rotation = quaternion_new(1, 0, 0, 0);
	param->screen = screen;
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
