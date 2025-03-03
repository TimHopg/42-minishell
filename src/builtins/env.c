/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 12:05:49 by npizzi            #+#    #+#             */
/*   Updated: 2025/01/27 19:50:06 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ms_env(t_msdata *msdata)
{
	char	**environment;
	int		i;

	environment = msdata->env;
	if (!environment)
		return (1);
	i = 0;
	while (environment[i])
	{
		if (ft_strchr(environment[i], '=') != NULL)
		{
			ft_dprintf(msdata->out_fd, "%s\n", environment[i]);
		}
		i++;
	}
	return (0);
}
