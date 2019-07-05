/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 11:33:26 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/05 13:27:26 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>

#include "camera.h"

void	camera_rotate(t_camera *camera, t_point rotation, float angle)
{
	float	ang[2];
	float	cosine[2];
	float	sine[2];
	t_quat	rot;

	ang[0] = (float)rotation.x * angle * (M_PI / 180);
	ang[1] = (float)rotation.y * angle * (M_PI / 180);
	cosine[0] = cos(ang[0]);
	cosine[1] = cos(ang[1]);
	sine[0] = sin(ang[0]);
	sine[1] = sin(ang[1]);
	rot.s = cosine[0] * cosine[1];
	rot.i = cosine[0] * sine[1];
	rot.j = sine[0] * cosine[1];
	rot.k = sine[0] * sine[1];
	quaternion_right_multiply(camera->rotation, &rot);
	camera->updated = 1;
}

void	camera_rotate_screen(t_camera *camera, t_point rotat, t_screen *screen)
{
	float	ang[2];
	float	cosine[2];
	float	sine[2];
	t_quat	rot;

	ang[0] = (float)(0 - rotat.x) / screen->width * camera->fov * (M_PI / 180);
	ang[1] = (float)rotat.y / screen->width * camera->fov * (M_PI / 180);
	cosine[0] = cos(ang[0]);
	cosine[1] = cos(ang[1]);
	sine[0] = sin(ang[0]);
	sine[1] = sin(ang[1]);
	rot.s = cosine[0] * cosine[1];
	rot.i = cosine[0] * sine[1];
	rot.j = sine[0] * cosine[1];
	rot.k = sine[0] * sine[1];
	quaternion_right_multiply(camera->rotation, &rot);
	camera->updated = 1;
}
