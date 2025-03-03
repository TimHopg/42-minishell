/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 08:56:34 by npizzi            #+#    #+#             */
/*   Updated: 2025/02/10 14:52:21 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Fetches env value if it exists
 *  @return Value of env or NULL
 */
char	*get_env_value(char *key, char **env)
{
	int	i;
	int	key_len;
	int	bytes_to_compare;

	i = 0;
	if (!key || !env)
		return (NULL);
	key_len = ft_strlen(key);
	while (env[i])
	{
		bytes_to_compare = ft_max(key_len, get_key_len(env[i]));
		if (ft_strncmp(key, env[i], bytes_to_compare) == 0)
		{
			if (env[i][key_len] == 0)
				return (NULL);
			if (env[i][key_len + 1] == 0)
				return ("");
			return (env[i] + key_len + 1);
		}
		i++;
	}
	return (NULL);
}

/** @brief Updates value of env var, if there's no equal sign this function
			overwrites the variable. If not found returns 2.
 */
int	update_var_value_overwrite(char *variable, char **env)
{
	int	i;
	int	key_len;
	int	bytes_to_compare;

	i = 0;
	key_len = get_key_len(variable);
	while (env[i])
	{
		bytes_to_compare = ft_max(key_len, get_key_len(env[i]));
		if (ft_strncmp(variable, env[i], bytes_to_compare) == 0)
		{
			safe_free(&env[i]);
			env[i] = ft_strdup(variable);
			if (!env[i])
				return (safe_free_array(&env), 1);
			return (0);
		}
		i++;
	}
	return (KEY_NOT_EXIST);
}

/** @brief Updates value of env var. If not found returns 2.
 */
int	update_variable_value(char *variable, char **env)
{
	int	i;
	int	key_len;
	int	bytes_to_compare;

	i = 0;
	key_len = get_key_len(variable);
	while (env[i])
	{
		bytes_to_compare = ft_max(key_len, get_key_len(env[i]));
		if (ft_strncmp(variable, env[i], bytes_to_compare) == 0)
		{
			if (variable[key_len] == 0)
				return (0);
			safe_free(&env[i]);
			env[i] = ft_strdup(variable);
			if (!env[i])
				return (safe_free_array(&env), 1);
			return (0);
		}
		i++;
	}
	return (KEY_NOT_EXIST);
}

/** @brief 
 */
char	**add_var_to_env(char *variable, char **env)
{
	int		i;
	char	**new_env;

	if (update_variable_value(variable, env) != KEY_NOT_EXIST)
		return (env);
	new_env = malloc((ft_array_size(env) + 2) * sizeof(char *));
	if (!new_env)
		return (safe_free_array(&env), NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[i] = ft_strdup(variable);
	if (!new_env[i])
		return (safe_free_array(&new_env), free(env), NULL);
	i++;
	new_env[i] = NULL;
	free(env);
	return (new_env);
}

char	**sort_array(char **environ_array)
{
	int		i;
	int		x;
	char	*temp;

	i = 0;
	while (environ_array && environ_array[i])
	{
		x = i + 1;
		while (environ_array[x])
		{
			if (ft_strncmp(environ_array[i], environ_array[x],
					ft_strlen(environ_array[x])) > 0)
			{
				temp = environ_array[i];
				environ_array[i] = environ_array[x];
				environ_array[x] = temp;
			}
			x++;
		}
		i++;
	}
	return (environ_array);
}
