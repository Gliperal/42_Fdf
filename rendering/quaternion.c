/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 19:53:55 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/02 22:22:18 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "quaternion.h"

t_quat		*quaternion_new(float s, float i, float j, float k)
{
	t_quat *q;

	q = (t_quat *)malloc(sizeof(t_quat));
	if (q == NULL)
		return (NULL);
	q->s = s;
	q->i = i;
	q->j = j;
	q->k = k;
	return (q);
}

t_quat		*quaternion_multiply(t_quat *q1, t_quat *q2)
{
	t_quat *prod;

	prod = (t_quat *)malloc(sizeof(t_quat));
	if (prod == NULL)
		return (NULL);
	prod->s = q1->s * q2->s - q1->i * q2->i - q1->j * q2->j - q1->k * q2->k;
	prod->i = q1->s * q2->i + q1->i * q2->s + q1->j * q2->k - q1->k * q2->j;
	prod->j = q1->s * q2->j - q1->i * q2->k + q1->j * q2->s + q1->k * q2->i;
	prod->k = q1->s * q2->k + q1->i * q2->j - q1->j * q2->i + q1->k * q2->s;
	return (prod);
}

void		quaternion_left_multiply(t_quat *q1, t_quat *q2)
{
	float s;
	float i;
	float j;

	s = q2->s * q1->s - q2->i * q1->i - q2->j * q1->j - q2->k * q1->k;
	i = q2->s * q1->i + q2->i * q1->s + q2->j * q1->k - q2->k * q1->j;
	j = q2->s * q1->j - q2->i * q1->k + q2->j * q1->s + q2->k * q1->i;
	q1->k = q2->s * q1->k + q2->i * q1->j - q2->j * q1->i + q2->k * q1->s;
	q1->j = j;
	q1->i = i;
	q1->s = s;
}

void		quaternion_right_multiply(t_quat *q1, t_quat *q2)
{
	float s;
	float i;
	float j;

	s = q1->s * q2->s - q1->i * q2->i - q1->j * q2->j - q1->k * q2->k;
	i = q1->s * q2->i + q1->i * q2->s + q1->j * q2->k - q1->k * q2->j;
	j = q1->s * q2->j - q1->i * q2->k + q1->j * q2->s + q1->k * q2->i;
	q1->k = q1->s * q2->k + q1->i * q2->j - q1->j * q2->i + q1->k * q2->s;
	q1->j = j;
	q1->i = i;
	q1->s = s;
}

t_matrix	*quaternion_to_matrix(t_quat *q)
{
	t_vertex	*cols[4];
	t_matrix	*matrix;

	cols[0] = vertex_new(1 - 2 * (q->j * q->j + q->k * q->k), 2 * \
		(q->i * q->j + q->k * q->s), 2 * (q->i * q->k - q->j * q->s), 0);
	cols[1] = vertex_new(2 * (q->i * q->j - q->k * q->s), 1 - 2 * \
			(q->i * q->i + q->k * q->k), 2 * (q->j * q->k + q->i * q->s), 0);
	cols[2] = vertex_new(2 * (q->i * q->k + q->j * q->s), 2 * \
		(q->j * q->k - q->i * q->s), 1 - 2 * (q->i * q->i + q->j * q->j), 0);
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
