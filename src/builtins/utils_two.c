/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 10:49:38 by npizzi            #+#    #+#             */
/*   Updated: 2025/02/10 16:32:06 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_key_len(char *variable)
{
	int		key_len;
	char	*value;

	value = ft_strchr(variable, '=');
	if (!value)
		return (ft_strlen(variable));
	key_len = value - variable;
	return (key_len);
}

bool	is_identifier_valid(char *identifier)
{
	int	i;

	i = 0;
	if ((identifier[0] >= '0' && identifier[0] <= '9') || identifier[0] == '=')
		return (false);
	while (identifier[i] && identifier[i] != '=')
	{
		if (!ft_isalnum(identifier[i]) && identifier[i] != '_')
		{
			if (i != 0 && identifier[i] == '+' && identifier[i + 1] == '=')
				return (true);
			else
				return (false);
		}
		i++;
	}
	return (true);
}

/*
 * Copies array of strings and returns as env_copy.
 */
char	**copy_env(t_msdata *msdata, char **env)
{
	int		i;
	char	**env_copy;

	if (!env)
		return (NULL);
	i = 0;
	env_copy = safe_calloc(msdata, (ft_array_size(env) + 1) * sizeof(char *));
	while (env[i])
	{
		env_copy[i] = ft_strdup(env[i]);
		if (!env_copy[i])
			return (safe_free_array(&env_copy), NULL);
		i++;
	}
	env_copy[i] = NULL;
	return (env_copy);
}

int	ft_array_size_char_incl(char **array, char c)
{
	int	array_size;
	int	i;

	array_size = 0;
	i = 0;
	if (!array)
		return (0);
	while (array[i])
	{
		if (ft_strchr(array[i], c))
			array_size++;
		i++;
	}
	return (array_size);
}

char	**copy_env_if(t_msdata *msdata, char **env)
{
	int		i;
	int		j;
	char	**env_copy;
	size_t	array_size;

	if (!env)
		return (NULL);
	i = 0;
	j = 0;
	array_size = ft_array_size_char_incl(env, '=');
	env_copy = safe_calloc(msdata, (array_size + 1) * sizeof(char *));
	while (env[i])
	{
		if (ft_strchr(env[i], '='))
		{
			env_copy[j] = ft_strdup(env[i]);
			if (!env_copy[j])
				return (safe_free_array(&env_copy), NULL);
			j++;
		}
		i++;
	}
	env_copy[j] = NULL;
	return (env_copy);
}
