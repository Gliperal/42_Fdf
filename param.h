/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 12:38:59 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/02 12:31:31 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARAM_H
# define PARAM_H

# include "map.h"
# include "quaternion.h"

typedef struct	s_camera
{
	int			x;
	int			y;
	int			z;
	t_quat		*rotation;
}				t_camera;

typedef struct	s_param
{
	t_map		*world;
	t_screen	*screen;
	t_camera	*camera;
	int			camera_updated;
	int			mouse_x;
	int			mouse_y;
	int			mouse1held;
	int			mouse2held;
}				t_param;

#endif
