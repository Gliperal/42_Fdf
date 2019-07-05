/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 11:33:26 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/04 17:03:04 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>

#include "camera.h"

static void	update_matrix(t_camera *camera)
{
	t_matrix *m_translate;
	t_matrix *m_rotate;
	t_matrix *m_proj;
	t_matrix *m_trtt;
	t_matrix *m_prtrtt;

	m_translate = translate_matrix(camera->position);
	m_rotate = rotation_matrix(camera->rotation);
	m_proj = opengl_projection_matrix(camera->fov, \
			camera->n, camera->f, camera->ar);
	m_trtt = matrix_multiply(m_rotate, m_translate);
	m_prtrtt = matrix_multiply(m_proj, m_trtt);
	free(m_translate);
	free(m_rotate);
	free(m_proj);
	free(m_trtt);
	if (m_prtrtt != NULL)
		camera->clip_matrix = m_prtrtt;
}

t_vertex	*camera_vertex_to_clip(t_camera *camera, t_vertex *vertex)
{
	if (camera->updated)
	{
		update_matrix(camera);
		camera->updated = 0;
	}
	return (transform_vertex(camera->clip_matrix, vertex));
}

t_camera	*camera_new(float fov, float n, float f, float ar)
{
	t_camera *camera;

	camera = (t_camera *)malloc(sizeof(t_camera));
	if (camera == NULL)
		return (NULL);
	camera->rotation = quaternion_new(1, 0, 0, 0);
	camera->position = vertex_new(0, 0, -20, 0);
	if (camera->rotation == NULL || camera->position == NULL)
	{
		free(camera->position);
		free(camera->rotation);
		free(camera);
		return (NULL);
	}
	camera->fov = fov;
	camera->n = n;
	camera->f = f;
	camera->ar = ar;
	camera->updated = 1;
	return (camera);
}

void	camera_move(t_camera *camera, const t_vertex *offset, float distance)
{
	t_vertex *rotated_offset;

	rotated_offset = quaternion_rotate_vertex(camera->rotation, offset);
	if (rotated_offset != NULL)
	{
		rotated_offset->x *= distance;
		rotated_offset->y *= distance;
		rotated_offset->z *= distance;
		vertex_move(camera->position, rotated_offset);
		free(rotated_offset);
		camera->updated = 1;
	}
}

void	camera_spin(t_camera *camera, float angle)
{
	t_quat	rot;
	float	rad;

	rad = angle * (M_PI / 180);
	rot.s = cos(rad);
	rot.i = 0;
	rot.j = 0;
	rot.k = sin(rad);
	quaternion_right_multiply(camera->rotation, &rot);
	camera->updated = 1;
}

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

void	camera_rotate_screen(t_camera *camera, t_point rotation, t_screen *screen)
{
	float	ang[2];
	float	cosine[2];
	float	sine[2];
	t_quat	rot;

	ang[0] = (float)(0 - rotation.x) / screen->width * camera->fov * (M_PI / 180);
	ang[1] = (float)rotation.y / screen->width * camera->fov * (M_PI / 180);
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
