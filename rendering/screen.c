/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screen.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 11:51:17 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/05 12:00:08 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "mlx_util.h"
#include "../minilibx_macos/mlx.h"

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
