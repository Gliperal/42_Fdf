/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 18:12:23 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/02 22:23:22 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>

#include "matrix.h"

t_vertex	*transform_vertex(t_matrix *matrix, t_vertex *vertex)
{
	t_vertex *result;
	t_vertex *col1;
	t_vertex *col2;
	t_vertex *col3;
	t_vertex *col4;

	col1 = vertex_scale(matrix->x, vertex->x);
	col2 = vertex_scale(matrix->y, vertex->y);
	col3 = vertex_scale(matrix->z, vertex->z);
	col4 = vertex_scale(matrix->w, vertex->w);
	result = NULL;
	if (col1 && col2 && col3 && col4)
		result = vertex_sum_four(col1, col2, col3, col4);
	free(col1);
	free(col2);
	free(col3);
	free(col4);
	return (result);
}

t_matrix	*matrix_multiply(t_matrix *m1, t_matrix *m2)
{
	t_matrix *result;

	result = (t_matrix *)malloc(sizeof(t_matrix));
	if (result == NULL)
		return (NULL);
	result->x = transform_vertex(m1, m2->x);
	result->y = transform_vertex(m1, m2->y);
	result->z = transform_vertex(m1, m2->z);
	result->w = transform_vertex(m1, m2->w);
	if (result->x && result->y && result->z && result->z)
		return (result);
	free(result->x);
	free(result->y);
	free(result->z);
	free(result->w);
	return (NULL);
}

t_matrix	*matrix_new(t_vtx *col1, t_vtx *col2, t_vtx *col3, t_vtx *col4)
{
	t_matrix *result;

	result = (t_matrix *)malloc(sizeof(t_matrix));
	if (result == NULL)
		return (NULL);
	result->x = col1;
	result->y = col2;
	result->z = col3;
	result->w = col4;
	return (result);
}

/*
** t_matrix	*opengl_projection_matrix_advanced(float fov, \
** float n, float f, float aspect_ratio)
** {
** 	float scale = tan(fov * 0.5 * M_PI / 180) * n;
** 	float r = aspect_ratio * scale;
** 	float l = 0 - r;
** 	float b = scale;
** 	float t = 0 - b;
** 	t_matrix *m_proj = matrix_new(
** 		vertex_new(2 * n / (r - l), 0, 0, 0),
** 		vertex_new(0, 2 * n / (b - t), 0, 0),
** 		vertex_new((r + l) / (r - l), (b + t) / (b - t), \
** (f + n) / (f - n), 1),
** 		vertex_new(0, 0, 0 - 2 * f * n / (f - n), 0)
** 	);
** 	// TODO malloc catch
** 	return (m_proj);
** }
*/

t_matrix	*opengl_projection_matrix(float fov, float n, float f, float ar)
{
	float		scale;
	float		r;
	float		b;
	t_vertex	*cols[4];
	t_matrix	*m_proj;

	scale = tan(fov * 0.5 * M_PI / 180) * n;
	r = ar * scale;
	b = scale;
	cols[0] = vertex_new(n / r, 0, 0, 0);
	cols[1] = vertex_new(0, n / b, 0, 0);
	cols[2] = vertex_new(0, 0, (f + n) / (f - n), 1);
	cols[3] = vertex_new(0, 0, 0 - 2 * f * n / (f - n), 0);
	m_proj = matrix_new(cols[0], cols[1], cols[2], cols[3]);
	if (cols[0] && cols[1] && cols[2] && cols[3] && m_proj)
		return (m_proj);
	free(cols[0]);
	free(cols[1]);
	free(cols[2]);
	free(cols[3]);
	free(m_proj);
	return (NULL);
}
