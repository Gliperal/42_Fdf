/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 19:53:55 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/06/30 21:59:02 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "mlx.h"
#include "mlx_util.h"

int	handle_mouse(int button, int x, int y, void *param)
{
	char c[3];
	c[0] = '0' + (button / 100) % 10;
	c[1] = '0' + (button / 10) % 10;
	c[2] = '0' + button % 10;
	write(1, c, 3);
	write(1, "\n", 1);
	return 0;
}

int	handle_key_press(int key, void *param)
{
	return 0;
}

int handle_key_release(int key, void *param)
{
	char c[3];
	c[0] = '0' + (key / 100) % 10;
	c[1] = '0' + (key / 10) % 10;
	c[2] = '0' + key % 10;
	write(1, c, 3);
	write(1, "\n", 1);
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
	screen->size_line = BPP * width;
	screen->bpp = BPP;
	screen->endian = MACOS_ENDIAN;
	screen->data = (void *)mlx_get_data_addr(screen->img_ptr, &screen->bpp, &screen->size_line, &screen->endian);
	if (screen->data == NULL)
		return (NULL);
	screen->width = width;
	screen->height = height;
	return (screen);
}

int main()
{
	t_screen *screen = new_screen(640, 480, "Hello world!");
	if (screen == NULL)
	{
		write(1, "oh no\n", 6);
		exit(1);
	}
	int	i = mlx_put_image_to_window(screen->mlx_ptr, screen->win_ptr, screen->img_ptr, 0, 0);
	mlx_do_key_autorepeatoff(screen->mlx_ptr);
	mlx_hook(screen->win_ptr, 2, 0, &handle_key_press, NULL);
	mlx_hook(screen->win_ptr, 3, 0, &handle_key_release, NULL);
	mlx_hook(screen->win_ptr, 4, 0, &handle_mouse, NULL);
	mlx_hook(screen->win_ptr, 17, 0, &handle_exit, NULL);
	mlx_loop(screen->mlx_ptr);
}
