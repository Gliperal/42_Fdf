/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/02 21:09:23 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/04 13:08:15 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERING_H
# define RENDERING_H

# include "mlx_util.h"
# include "../libft/libft.h"
# include "quaternion.h"
# include "vertex.h"
# include "matrix.h"
# include "camera.h"
# include "cpoint.h"

void	ft_draw_line(t_screen *screen, t_cpoint src, t_cpoint dst);

#endif
