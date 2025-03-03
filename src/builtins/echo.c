/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 18:15:41 by thopgood          #+#    #+#             */
/*   Updated: 2025/01/27 19:49:58 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Checks if the -n flag was set, flag can be set with -n
// user can put any amount of n to set the flag
bool	option_n(char *first_arg)
{
	int	i;

	if (!first_arg || first_arg[0] != '-' || first_arg[1] != 'n')
		return (false);
	i = 1;
	while (first_arg[i])
	{
		if (first_arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

int	ms_echo(t_msdata *msdata, char **args)
{
	bool	flag_n_set;
	int		i;

	if (!args[0])
		return (ft_putchar_fd('\n', msdata->out_fd), 0);
	flag_n_set = option_n(args[0]);
	i = flag_n_set;
	while (option_n(args[i]))
		i++;
	while (args[i])
	{
		ft_putstr_fd(args[i], msdata->out_fd);
		if (args[i + 1])
			ft_putchar_fd(' ', msdata->out_fd);
		i++;
	}
	if (!flag_n_set)
		ft_putchar_fd('\n', msdata->out_fd);
	return (0);
}
