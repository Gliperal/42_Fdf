/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertex.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 18:12:48 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/01 21:48:25 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VERTEX_H
# define VERTEX_H

typedef struct	s_vertex
{
	float		x;
	float		y;
	float		z;
	float		w;
}				t_vertex;

t_vertex	*vertex_new(float x, float y, float z, float w);
t_vertex	*vertex_scale(t_vertex *vertex, float scale);
t_vertex	*vertex_sum_four(t_vertex *v1, t_vertex *v2, t_vertex *v3, t_vertex *v4);

#endif
