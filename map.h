/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 20:40:16 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/04 18:02:12 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_H
# define MAP_H

# include "rendering/rendering.h"

typedef struct	s_map
{
	int			width;
	int			height;
	t_vertex	***data;
	float		min;
	float		max;
	float		z_scale;
}				t_map;

#endif
