/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 12:05:59 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/03 12:16:57 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include "matrix.h"
# include "quaternion.h"

typedef struct	s_camera
{
	float		fov;
	float		n;
	float		f;
	float		ar;
	t_quat		*position;
	t_quat		*rotation;
	t_matrix	*clip_matrix;
	int			updated;
}				t_camera;

t_vertex		*camera_vertex_to_clip(t_camera *camera, t_vertex *vertex);
t_camera		*camera_new(float fov, float n, float f, float ar);

#endif