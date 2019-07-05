/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/03 13:39:46 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/04 22:56:25 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>

#include "input.h"
#include "minilibx_macos/mlx.h"

static void	send_update(t_input *input)
{
	int key;
	int button;

	(*input->on_update)(input->param);
	button = 0;
	while (button < MAX_BUTTONS)
	{
		if (input->button_states[button] == PRESSED)
			input->button_states[button] = HELD;
		else if (input->button_states[button] == RELEASED)
			input->button_states[button] = NOT_HELD;
		button++;
	}
	key = 0;
	while (key < MAX_KEYS)
	{
		if (input->key_states[key] == PRESSED)
			input->key_states[key] = HELD;
		else if (input->key_states[key] == RELEASED)
			input->key_states[key] = NOT_HELD;
		key++;
	}
	input->mouse_moved.x = 0;
	input->mouse_moved.y = 0;
}

# define UPDATE_INTERVAL 16666666L
# define NSEC_IN_SEC 1000000000L

static long	time_subtract(struct timespec a, struct timespec b)
{
	return ((a.tv_sec - b.tv_sec) * NSEC_IN_SEC + a.tv_nsec - b.tv_nsec);
}

static struct timespec	time_add(struct timespec t, long n)
{
	t.tv_nsec += n;
	while (t.tv_nsec < 0)
	{
		t.tv_sec--;
		t.tv_nsec += NSEC_IN_SEC;
	}
	while (t.tv_nsec > NSEC_IN_SEC)
	{
		t.tv_sec++;
		t.tv_nsec -= NSEC_IN_SEC;
	}
	return t;
}

static int	handle_loop(t_input *input)
{
	struct timespec	time;
	long			diff;

	clock_gettime(CLOCK_REALTIME, &time);
	diff = time_subtract(time, input->next_update_at);
	if (diff > 0)
	{
		send_update(input);
		input->fps = (float) NSEC_IN_SEC / time_subtract(input->next_update_at, input->last_update_at);
		input->last_update_at = input->next_update_at;
		if (diff > UPDATE_INTERVAL)
			input->next_update_at = time;
		else
			input->next_update_at = time_add(input->next_update_at, UPDATE_INTERVAL);
	}
	return (0);
}

static int	handle_key_press(int key, t_input *input)
{
	if (key < 0 || key > MAX_KEYS)
		return (1);
	input->key_states[key] = PRESSED;
	handle_loop(input);
	return (0);
}

static int	handle_key_release(int key, t_input *input)
{
	if (key < 0 || key > MAX_KEYS)
		return (1);
	input->key_states[key] = RELEASED;
	handle_loop(input);
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
	if (button < 0 || button > MAX_BUTTONS)
		return (1);
	input->button_states[button] = PRESSED;
	update_mouse_position(input, x, y);
	handle_loop(input);
	return (0);
}

static int	handle_mouse_release(int button, int x, int y, t_input *input)
{
	if (button < 0 || button > MAX_BUTTONS)
		return (1);
	input->button_states[button] = RELEASED;
	update_mouse_position(input, x, y);
	handle_loop(input);
	return (0);
}

static int	handle_mouse_move(int x, int y, t_input *input)
{
	int moved;

	moved = update_mouse_position(input, x, y);
	if (moved)
		handle_loop(input);
	return (0);
}

static int	handle_expose(t_input *input)
{
	input->exposed = 1;
	handle_loop(input);
	return (0);
}

static int	handle_exit(void)
{
	exit(0);
}

t_input	*input_new(void (*on_update)(void *), void *param, t_screen *screen)
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
	i = 0;
	while (i < MAX_KEYS + 1)
	{
		input->key_states[i] = NOT_HELD;
		i++;
	}
	input->on_update = on_update;
	input->param = param;
	input->exposed = 0;
	clock_gettime(CLOCK_REALTIME, &input->next_update_at);
//	mlx_do_key_autorepeatoff(screen->mlx_ptr);
	mlx_hook(screen->win_ptr, 2, 0, &handle_key_press, input);
	mlx_hook(screen->win_ptr, 3, 0, &handle_key_release, input);
	mlx_hook(screen->win_ptr, 4, 0, &handle_mouse_press, input);
	mlx_hook(screen->win_ptr, 5, 0, &handle_mouse_release, input);
	mlx_hook(screen->win_ptr, 6, 0, &handle_mouse_move, input);
	mlx_hook(screen->win_ptr, 12, 0, &handle_expose, input);
	mlx_hook(screen->win_ptr, 17, 0, &handle_exit, NULL);
	mlx_loop_hook(screen->mlx_ptr, &handle_loop, input);
	return (input);
}
