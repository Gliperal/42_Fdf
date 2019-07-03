/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 13:57:37 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/03 14:39:10 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include "libft/libft.h"
# include "rendering/mlx_util.h"

typedef char			t_button_state;

# define NOT_HELD 0
# define HELD 1
# define PRESSED 2
# define RELEASED 3

typedef struct	s_input
{
	t_button_state	button_states[MAX_BUTTONS + 1];
	int				mouse_yet_to_move;
	t_point			mouse;
	t_point			mouse_moved;
	void			(*on_update)(void *);
	void			*param;
}					t_input;

t_input	*input_new(void (*on_update)(void *), void *param, MLX_WIN *win_ptr);

#endif
