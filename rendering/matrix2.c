/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 18:12:23 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/05 13:29:33 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>

#include "../libft/libft.h"
#include "matrix.h"

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

void		matrix_print(t_matrix *matrix)
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

t_matrix	*scale_matrix(float scale)
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

t_matrix	*identity_matrix(void)
{
	return (scale_matrix(1));
}
