/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/01 12:38:59 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/05 13:05:42 by nwhitlow         ###   ########.fr       */
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
	int			fog;
}				t_param;

void			render(t_param *param);

#endif
