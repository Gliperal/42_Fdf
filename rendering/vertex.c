/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 18:12:23 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/02 22:11:04 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "vertex.h"

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
	return (vertex_new(vertex->x * scale, vertex->y * scale, \
			vertex->z * scale, vertex->w * scale));
}

t_vertex	*vertex_sum_four(t_vtx *v1, t_vtx *v2, t_vtx *v3, t_vtx *v4)
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
