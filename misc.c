/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 13:01:25 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/05 13:05:06 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

int		key_down(t_input *input, int button)
{
	return (input->key_states[button] == HELD ||
			input->key_states[button] == PRESSED);
}

int		button_down(t_input *input, int button)
{
	return (input->button_states[button] == HELD ||
			input->button_states[button] == PRESSED);
}

void	do_exit(char *err, int exit_code)
{
	ft_putendl(err);
	exit(exit_code);
}
