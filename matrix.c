/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 18:12:23 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/01 20:10:50 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "matrix.h"

t_vertex	*vertex_new(float x, float y, float z, float w)
{
	t_vertex *v;

	v = (t_vertex *)malloc(sizeof(t_vertex));
	if (v == NULL)
		return (NULL);
	v->x = x;
	v->y = y;
	v->z = z;
	v->w = w;
	return (v);
}

t_vertex	*vertex_scale(t_vertex *vertex, float scale)
{
	return vertex_new(vertex->x * scale, vertex->y * scale, vertex->z * scale, vertex->w * scale);
}

t_vertex	*vertex_sum_four(t_vertex *v1, t_vertex *v2, t_vertex *v3, t_vertex *v4)
{
	t_vertex *v;

	v = (t_vertex *)malloc(sizeof(t_vertex));
	if (v == NULL)
		return (NULL);
	v->x = v1->x + v2->x + v3->x + v4->x;
	v->y = v1->y + v2->y + v3->y + v4->y;
	v->z = v1->z + v2->z + v3->z + v4->z;
	v->w = v1->w + v2->w + v3->w + v4->w;
	return (v);
}

#include <stdio.h>
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
/*	printf("%f, %f, %f, %f\n", col1->x, col1->y, col1->z, col1->w);
	printf("%f, %f, %f, %f\n", col2->x, col2->y, col2->z, col2->w);
	printf("%f, %f, %f, %f\n", col3->x, col3->y, col3->z, col3->w);
	printf("%f, %f, %f, %f\n", col4->x, col4->y, col4->z, col4->w);
*/	result = NULL;
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

t_matrix	*matrix_new(t_vertex *col1, t_vertex *col2, t_vertex *col3, t_vertex *col4)
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

int main()
{
	t_vertex *v = vertex_new(10, 20, 30, 1);
	t_matrix *m1 = matrix_new(
		vertex_new(0.6, 0, 0.8, 0),
		vertex_new(0, 1, 0, 0),
		vertex_new(-0.8, 0, 0.6, 0),
		vertex_new(0, 0, 0, 1)
	);
	t_matrix *m2 = matrix_new(
		vertex_new(1, 0, 0, 0),
		vertex_new(0, 1, 0, 0),
		vertex_new(0, 0, 1, 0),
		vertex_new(1.2, 3.4, 4.5, 1)
	);
	t_vertex *v2 = transform_vertex(m1, v);
	printf("%f, %f, %f, %f\n", v2->x, v2->y, v2->z, v2->w);
	t_vertex *v3 = transform_vertex(m2, v2);
	printf("%f, %f, %f, %f\n", v3->x, v3->y, v3->z, v3->w);
	t_matrix *m3 = matrix_multiply(m2, m1);
	t_vertex *v4 = transform_vertex(m3, v);
	printf("%f, %f, %f, %f\n", v4->x, v4->y, v4->z, v4->w);
}
