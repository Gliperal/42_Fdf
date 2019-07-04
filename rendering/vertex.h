/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 18:12:48 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/04 12:55:54 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VERTEX_H
# define VERTEX_H

typedef struct		s_vertex
{
	float			x;
	float			y;
	float			z;
	float			w;
	int				color;
}					t_vertex;

typedef t_vertex	t_vtx;
typedef t_vertex	t_v;

t_vertex			*vertex_new(float x, float y, float z, float w);
t_vertex			*vertex_scale(t_vertex *vertex, float scale);
t_vertex			*vertex_opposite(t_vertex *vertex);
void				vertex_move(t_vertex *this, t_vertex *offset);
t_vertex			*vertex_sum_four(t_vtx *v1, t_vtx *v2, t_vtx *v3, t_v *v4);

#endif
