/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 18:12:48 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/04 18:07:21 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATRIX_H
# define MATRIX_H

# include "vertex.h"
# include "quaternion.h"

typedef struct	s_matrix
{
	t_vertex	*x;
	t_vertex	*y;
	t_vertex	*z;
	t_vertex	*w;
}				t_matrix;

t_matrix		*matrix_new(t_vtx *col1, t_vtx *col2, t_vtx *col3, t_vtx *col4);
t_vertex		*transform_vertex(t_matrix *matrix, t_vertex *vertex);
t_matrix		*matrix_multiply(t_matrix *m1, t_matrix *m2);
t_matrix		*translate_matrix(t_vertex *anti_offset);
t_matrix		*rotation_matrix(t_quat *q);
t_matrix		*opengl_projection_matrix(float fv, float n, float f, float a);
void			matrix_print(t_matrix *matrix);
t_matrix		*identity_matrix();
t_matrix		*scale_matrix(float scale);

#endif
