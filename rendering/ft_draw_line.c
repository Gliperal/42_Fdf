/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_draw_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/02 16:09:11 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/04 23:17:24 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_util.h"
#include "../libft/libft.h"
#include "cpoint.h"

static void	screen_put(t_screen *screen, int x, int y, int color)
{
	int	offset;

	if (x < 0 || x >= screen->width)
		return ;
	if (y < 0 || y >= screen->height)
		return ;
	offset = (screen->width * y + x) * screen->bpp / 8;
	screen->data[offset] = color & 0xff;
	screen->data[offset + 1] = (color >> 8) & 0xff;
	screen->data[offset + 2] = (color >> 16) & 0xff;
	screen->data[offset + 3] = color >> 24;
}

static int	inbetween_color(int c1, int c2, int n, int d)
{
	int rgba1[4];
	int rgba2[4];
	int rgba3[4];

	if (d == 0)
		return (c1);
	rgba1[0] = c1 >> 24;
	rgba1[1] = (c1 >> 16) & 255;
	rgba1[2] = (c1 >> 8) & 255;
	rgba1[3] = c1 & 255;
	rgba2[0] = c2 >> 24;
	rgba2[1] = (c2 >> 16) & 255;
	rgba2[2] = (c2 >> 8) & 255;
	rgba2[3] = c2 & 255;
	rgba3[0] = ((rgba1[0] * (d - n)) + (rgba2[0] * n)) / d;
	rgba3[1] = ((rgba1[1] * (d - n)) + (rgba2[1] * n)) / d;
	rgba3[2] = ((rgba1[2] * (d - n)) + (rgba2[2] * n)) / d;
	rgba3[3] = ((rgba1[3] * (d - n)) + (rgba2[3] * n)) / d;
	return ((rgba3[0] << 24) | (rgba3[1] << 16) | (rgba3[2] << 8) | rgba3[3]);
}

static void	draw_hline(t_screen *screen, t_cpoint src, t_cpoint dst)
{
	int	xdiff;
	int	ydiff;
	int	ydir;
	int	m;

	xdiff = dst.x - src.x;
	ydiff = ft_abs(dst.y - src.y);
	ydir = (dst.y > src.y) ? 1 : -1;
	m = 0;
	while (src.x <= dst.x)
	{
		m += ydiff;
		if (m >= xdiff)
		{
			m -= xdiff;
			src.y += ydir;
		}
		screen_put(screen, src.x, src.y, inbetween_color(\
					dst.color, src.color, dst.x - src.x, xdiff));
		src.x++;
	}
}

static void	draw_vline(t_screen *screen, t_cpoint src, t_cpoint dst)
{
	int	ydiff;
	int	xdiff;
	int	xdir;
	int	m;

	ydiff = dst.y - src.y;
	xdiff = ft_abs(dst.x - src.x);
	xdir = (dst.x > src.x) ? 1 : -1;
	m = 0;
	while (src.y <= dst.y)
	{
		m += xdiff;
		if (m >= ydiff)
		{
			m -= ydiff;
			src.x += xdir;
		}
		screen_put(screen, src.x, src.y, inbetween_color(\
					dst.color, src.color, dst.y - src.y, ydiff));
		src.y++;
	}
}

void		ft_draw_line(t_screen *screen, t_cpoint src, t_cpoint dst)
{
	int	diffx;
	int	diffy;

	diffx = ft_abs(dst.x - src.x);
	diffy = ft_abs(dst.y - src.y);
	if (diffy > diffx)
	{
		if (src.y > dst.y)
			draw_vline(screen, dst, src);
		else
			draw_vline(screen, src, dst);
	}
	else
	{
		if (src.x > dst.x)
			draw_hline(screen, dst, src);
		else
			draw_hline(screen, src, dst);
	}
}
