/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_draw_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/02 16:09:11 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/03 21:17:14 by nwhitlow         ###   ########.fr       */
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
	// TODO Maybe change screen->data to integer array
	offset = (screen->width * y + x) * screen->bpp / 8;
	screen->data[offset] = color & 0xff;
	screen->data[offset + 1] = (color >> 8) & 0xff;
	screen->data[offset + 2] = (color >> 16) & 0xff;
	screen->data[offset + 3] = color >> 24;
}

//	astep = ((dst >> 24) - (src >> 24)) / steps;
//	rstep = ((dst >> 16) & 255 - (src >> 16) & 255) / steps;
//	gstep = ((dst >> 8) & 255 - (src >> 8) & 255) / steps;
//	bstep = (dst & 255 - src & 255) / steps;

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
		screen_put(screen, src.x, src.y, src.color);
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
		screen_put(screen, src.x, src.y, src.color);
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
