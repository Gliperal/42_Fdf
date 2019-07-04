/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 16:27:13 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/03 18:43:59 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUATERNION_H
# define QUATERNION_H

# include "vertex.h"

typedef struct	s_quat
{
	float		s;
	float		i;
	float		j;
	float		k;
}				t_quat;

t_quat			*quaternion_new(float s, float i, float j, float k);
t_quat			*quaternion_multiply(t_quat *q1, t_quat *q2);
void			quaternion_left_multiply(t_quat *mplicand, t_quat *mplier);
void			quaternion_right_multiply(t_quat *mplicand, t_quat *mplier);
void			quaternion_rotate_vertex(t_quat *q, t_vertex *vertex);

#endif
