/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/30 19:53:55 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/01 16:31:25 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "quaternion.h"

t_quat	*quaternion_new(float s, float i, float j, float k)
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

t_quat	*quaternion_multiply(t_quat *q1, t_quat *q2)
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

void	quaternion_left_multiply(t_quat *q1, t_quat *q2)
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

void	quaternion_right_multiply(t_quat *q1, t_quat *q2)
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
