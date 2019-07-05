/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 11:33:26 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/05 13:30:37 by nwhitlow         ###   ########.fr       */
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
	t_matrix *m_product[3];

	m_translate = translate_matrix(camera->position);
	m_rotate = rotation_matrix(camera->rotation);
	m_proj = opengl_projection_matrix(camera->fov, \
			camera->n, camera->f, camera->ar);
	m_product[0] = matrix_multiply(m_translate, camera->pre_transformation);
	m_product[1] = matrix_multiply(m_rotate, m_product[0]);
	m_product[2] = matrix_multiply(m_proj, m_product[1]);
	free(m_translate);
	free(m_rotate);
	free(m_proj);
	free(m_product[1]);
	if (m_product[2] == NULL)
		return ;
	free(camera->clip_matrix);
	camera->clip_matrix = m_product[2];
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
	camera->pre_transformation = identity_matrix();
	if (camera->rotation == NULL || camera->position == NULL || \
			camera->pre_transformation == NULL)
	{
		free(camera->pre_transformation);
		free(camera->position);
		free(camera->rotation);
		free(camera);
		return (NULL);
	}
	camera->fov = fov;
	camera->n = n;
	camera->f = f;
	camera->ar = ar;
	camera->clip_matrix = NULL;
	camera->updated = 1;
	return (camera);
}

void		camera_move(t_camera *camera, const t_vertex *offset, float dist)
{
	t_vertex *rotated_offset;

	rotated_offset = quaternion_rotate_vertex(camera->rotation, offset);
	if (rotated_offset != NULL)
	{
		rotated_offset->x *= dist;
		rotated_offset->y *= dist;
		rotated_offset->z *= dist;
		vertex_move(camera->position, rotated_offset);
		free(rotated_offset);
		camera->updated = 1;
	}
}

void		camera_spin(t_camera *camera, float angle)
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
