/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 12:05:59 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/04 16:37:47 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include "../libft/libft.h"
# include "matrix.h"
# include "quaternion.h"
# include "mlx_util.h"

typedef struct	s_camera
{
	float		fov;
	float		n;
	float		f;
	float		ar;
	t_vertex	*position;
	t_quat		*rotation;
	t_matrix	*clip_matrix;
	int			updated;
}				t_camera;

t_vertex		*camera_vertex_to_clip(t_camera *camera, t_vertex *vertex);
t_camera		*camera_new(float fov, float n, float f, float ar);
void			camera_rotate(t_camera *camera, t_point rotation, float angle);
void			camera_move(t_camera *camera, const t_vertex *offset, float distance);
void			camera_spin(t_camera *camera, float angle);
void			camera_rotate_screen(t_camera *camera, t_point rotation, t_screen *screen);

#endif
