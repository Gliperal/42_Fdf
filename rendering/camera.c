/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 11:33:26 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/03 13:06:49 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "camera.h"

static void	update_matrix(t_camera *camera)
{
	t_matrix *m_translate = translate_matrix(camera->position);
	t_matrix *m_rotate = quaternion_to_matrix(camera->rotation);
	t_matrix *m_proj = opengl_projection_matrix(camera->fov, camera->n, camera->f, camera->ar);
	t_matrix *m_trtt = matrix_multiply(m_rotate, m_translate);
	t_matrix *m_prtrtt = matrix_multiply(m_proj, m_trtt);
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
	t_camera *camera = (t_camera *)malloc(sizeof(t_camera));
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
