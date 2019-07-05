/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/05 13:01:07 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/07/05 13:02:38 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MISC_H
# define MISC_H

int		key_down(t_input *input, int button);
int		button_down(t_input *input, int button);
void	do_exit(char *err, int exit_code);

#endif
