/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_draw_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/02 16:09:11 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/03 13:21:48 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_util.h"
#include "../libft/libft.h"

static void	screen_put(t_screen *screen, int x, int y, int color)
{
	int	offset;

	if (x < 0 || x >= screen->width)
		return ;
	if (y < 0 || y >= screen->height)
		return ;
	// TODO Maybe change screen->data to integer array
	offset = (screen->width * y + x) * screen->bpp / 8;
	screen->data[offset + 0] = color & 0xff;
	screen->data[offset + 1] = (color >> 2) & 0xff;
	screen->data[offset + 2] = (color >> 4) & 0xff;
}

static void	draw_hline(t_screen *screen, t_point src, t_point dst, int color)
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
		screen_put(screen, src.x, src.y, color);
		src.x++;
	}
}

static void	draw_vline(t_screen *screen, t_point src, t_point dst, int color)
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
		screen_put(screen, src.x, src.y, color);
		src.y++;
	}
}

void		ft_draw_line(t_screen *screen, t_point src, t_point dst, int color)
{
	int	diffx;
	int	diffy;

	diffx = ft_abs(dst.x - src.x);
	diffy = ft_abs(dst.y - src.y);
	if (diffy > diffx)
	{
		if (src.y > dst.y)
			draw_vline(screen, dst, src, color);
		else
			draw_vline(screen, src, dst, color);
	}
	else
	{
		if (src.x > dst.x)
			draw_hline(screen, dst, src, color);
		else
			draw_hline(screen, src, dst, color);
	}
}
