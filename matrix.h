/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 18:12:48 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/01 20:05:08 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATRIX_H
# define MATRIX_H

typedef struct	s_vertex
{
	float		x;
	float		y;
	float		z;
	float		w;
}				t_vertex;

typedef struct	s_matrix
{
	t_vertex	*x;
	t_vertex	*y;
	t_vertex	*z;
	t_vertex	*w;
}				t_matrix;

#endif
