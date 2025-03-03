/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 12:08:23 by npizzi            #+#    #+#             */
/*   Updated: 2025/02/10 16:39:20 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset_variable(char *key, char **environment)
{
	int	i;
	int	key_len;
	int	bytes_to_compare;

	i = 0;
	if (!key || ft_strchr(key, '='))
		return ;
	key_len = ft_strlen(key);
	while (environment[i])
	{
		bytes_to_compare = ft_max(key_len, get_key_len(environment[i]));
		if (ft_strncmp(key, environment[i], bytes_to_compare) == 0)
		{
			safe_free(&environment[i]);
			environment[i] = environment[i + 1];
			while (environment[i])
			{
				environment[i] = environment[i + 1];
				i++;
			}
			break ;
		}
		i++;
	}
}

int	ms_unset(char **environment, char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		unset_variable(args[i], environment);
		i++;
	}
	return (0);
}
