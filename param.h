/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 12:38:59 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/04 15:24:19 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARAM_H
# define PARAM_H

# include <time.h>

# include "map.h"
# include "rendering/rendering.h"
# include "input.h"

typedef struct	s_param
{
	t_map		*world;
	t_screen	*screen;
	t_camera	*camera;
	t_input		*input;
	struct timespec		last_update_at;
	struct timespec		next_update_at;
	float		fps;
}				t_param;

#endif
