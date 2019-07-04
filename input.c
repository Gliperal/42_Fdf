/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 13:39:46 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/04 13:59:22 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"
#include "minilibx_macos/mlx.h"

static void	send_update(t_input *input)
{
	int key;

	(*input->on_update)(input->param);
	key = 0;
	while (key < MAX_BUTTONS)
	{
		if (input->button_states[key] == PRESSED)
			input->button_states[key] = HELD;
		else if (input->button_states[key] == RELEASED)
			input->button_states[key] = NOT_HELD;
		key++;
	}
	input->mouse_moved.x = 0;
	input->mouse_moved.y = 0;
}

static int	handle_key_press(int key, t_input *input)
{
	if (key < 0 || key > MAX_BUTTONS)
		return (1);
	input->button_states[key] = PRESSED;
	send_update(input);
	return (0);
}

static int	handle_key_release(int key, t_input *input)
{
	if (key < 0 || key > MAX_BUTTONS)
		return (1);
	input->button_states[key] = RELEASED;
	send_update(input);
	return (0);
}

static int	update_mouse_position(t_input *input, int x, int y)
{
	if (input->mouse_yet_to_move)
	{
		input->mouse_moved.x = 0;
		input->mouse_moved.y = 0;
		input->mouse_yet_to_move = 0;
	}
	else
	{
		input->mouse_moved.x += x - input->mouse.x;
		input->mouse_moved.y += y - input->mouse.y;
	}
	input->mouse.x = x;
	input->mouse.y = y;
	return (input->mouse_moved.x != 0 || input->mouse_moved.y != 0);
}

static int	handle_mouse_press(int button, int x, int y, t_input *input)
{
	handle_key_press(button, input);
	update_mouse_position(input, x, y);
	send_update(input);
	return (0);
}

static int	handle_mouse_release(int button, int x, int y, t_input *input)
{
	handle_key_release(button, input);
	update_mouse_position(input, x, y);
	send_update(input);
	return (0);
}

static int	handle_mouse_move(int x, int y, t_input *input)
{
	int moved;

	moved = update_mouse_position(input, x, y);
	if (moved)
		send_update(input);
	return (0);
}

// TODO REMOVE
#include <unistd.h>

static int	handle_expose(t_input *input)
{
	// TODO force redraw
	write(1, "expose\n", 7);
	send_update(input);
	return (0);
}

static int	handle_exit(void)
{
	exit(0);
}

t_input	*input_new(void (*on_update)(void *), void *param, MLX_WIN *win_ptr)
{
	t_input	*input;
	int		i;

	input = (t_input *)malloc(sizeof(t_input));
	if (input == NULL)
		return (NULL);
	input->mouse_yet_to_move = 1;
	i = 0;
	while (i < MAX_BUTTONS + 1)
	{
		input->button_states[i] = NOT_HELD;
		i++;
	}
	input->on_update = on_update;
	input->param = param;
	mlx_hook(win_ptr, 2, 0, &handle_key_press, input);
	mlx_hook(win_ptr, 3, 0, &handle_key_release, input);
	mlx_hook(win_ptr, 4, 0, &handle_mouse_press, input);
	mlx_hook(win_ptr, 5, 0, &handle_mouse_release, input);
	mlx_hook(win_ptr, 6, 0, &handle_mouse_move, input);
	mlx_hook(win_ptr, 12, 0, &handle_expose, input);
	mlx_hook(win_ptr, 17, 0, &handle_exit, NULL);
	return (input);
}
