/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 18:12:23 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/02 21:09:14 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

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

/*
#include <stdio.h>
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
*/
