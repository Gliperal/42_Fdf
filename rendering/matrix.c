/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 18:12:23 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/04 23:12:55 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>

#include "../libft/libft.h"
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
	{
		result = vertex_sum_four(col1, col2, col3, col4);
		result->color = vertex->color;
	}
	free(col1);
	free(col2);
	free(col3);
	free(col4);
	return (result);
}

t_matrix	*matrix_multiply(t_matrix *m1, t_matrix *m2)
{
	t_matrix *result;

	if (m1 == NULL || m2 == NULL)
		return (NULL);
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

t_matrix	*translate_matrix(t_vertex *anti_offset)
{
	t_vertex *cols[4];
	t_matrix *m_translate;

	if (anti_offset == NULL)
		return (NULL);
	cols[0] = vertex_new(1, 0, 0, 0);
	cols[1] = vertex_new(0, 1, 0, 0);
	cols[2] = vertex_new(0, 0, 1, 0);
	cols[3] = vertex_opposite(anti_offset);
	if (!cols[0] || !cols[1] || !cols[2] || !cols[3])
	{
		free(cols[0]);
		free(cols[1]);
		free(cols[2]);
		free(cols[3]);
		return (NULL);
	}
	cols[3]->w = 1;
	m_translate = matrix_new(cols[0], cols[1], cols[2], cols[3]);
	return (m_translate);
}

/*
** Note that this rotation matrix is derived from the inverse of q (which
** corresponds to the camera rotation).
*/

t_matrix	*rotation_matrix(t_quat *q)
{
	t_vertex	*cols[4];
	t_matrix	*matrix;

	cols[0] = vertex_new(1 - 2 * (q->j * q->j + q->k * q->k), 2 * \
		(q->i * q->j - q->k * q->s), 2 * (q->i * q->k + q->j * q->s), 0);
	cols[1] = vertex_new(2 * (q->i * q->j + q->k * q->s), 1 - 2 * \
			(q->i * q->i + q->k * q->k), 2 * (q->j * q->k - q->i * q->s), 0);
	cols[2] = vertex_new(2 * (q->i * q->k - q->j * q->s), 2 * \
		(q->j * q->k + q->i * q->s), 1 - 2 * (q->i * q->i + q->j * q->j), 0);
	cols[3] = vertex_new(0, 0, 0, 1);
	matrix = matrix_new(cols[0], cols[1], cols[2], cols[3]);
	if (!cols[0] || !cols[1] || !cols[2] || !cols[3] || !matrix)
	{
		free(cols[0]);
		free(cols[1]);
		free(cols[2]);
		free(cols[3]);
		free(matrix);
		return (NULL);
	}
	return (matrix);
}

void	matrix_print(t_matrix *matrix)
{
	t_vertex *v1;
	t_vertex *v2;
	t_vertex *v3;
	t_vertex *v4;

	if (matrix == NULL)
		ft_putstr("[NULL]\n");
	v1 = matrix->x;
	v2 = matrix->y;
	v3 = matrix->z;
	v4 = matrix->w;
	ft_printf("[%7.2f %7.2f %7.2f %7.2f]\n", v1->x, v2->x, v3->x, v4->x);
	ft_printf("[%7.2f %7.2f %7.2f %7.2f]\n", v1->y, v2->y, v3->y, v4->y);
	ft_printf("[%7.2f %7.2f %7.2f %7.2f]\n", v1->z, v2->z, v3->z, v4->z);
	ft_printf("[%7.2f %7.2f %7.2f %7.2f]\n", v1->w, v2->w, v3->w, v4->w);
}

t_matrix		*scale_matrix(float scale)
{
	t_vertex *cols[4];
	t_matrix *m_identity;

	cols[0] = vertex_new(scale, 0, 0, 0);
	cols[1] = vertex_new(0, scale, 0, 0);
	cols[2] = vertex_new(0, 0, scale, 0);
	cols[3] = vertex_new(0, 0, 0, scale);
	m_identity = matrix_new(cols[0], cols[1], cols[2], cols[3]);
	if (cols[0] && cols[1] && cols[2] && cols[3] && m_identity)
		return (m_identity);
	free(cols[0]);
	free(cols[1]);
	free(cols[2]);
	free(cols[3]);
	free(m_identity);
	return (NULL);
}

t_matrix		*identity_matrix(void)
{
	return (scale_matrix(1));
}
