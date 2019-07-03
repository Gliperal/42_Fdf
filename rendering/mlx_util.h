/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_util.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 21:19:41 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/03 14:29:07 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_UTIL_H
# define MLX_UTIL_H

# define LCLICK 1
# define RCLICK 2
# define MWHEELDN 4
# define MWHEELUP 5
# define MWHEELRT 6
# define MWHEELLF 7
# define ENTER 36
# define SPACE 49
# define ESC 53
# define LEFT 123
# define RIGHT 124
# define DOWN 125
# define UP 126
# define CTRL 256
# define SHIFT 257
# define RSHIFT 258

# define MAX_BUTTONS 124

# define MLX void
# define MLX_WIN void
# define MLX_IMG void

typedef unsigned char	byte;

typedef struct	s_screen
{
	int			width;
	int			height;
	MLX			*mlx_ptr;
	MLX_WIN		*win_ptr;
	MLX_IMG		*img_ptr;
	byte		*data;
	int			bpp;
	int			size_line;
	int			endian;
}				t_screen;

#endif
