/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 11:04:37 by npizzi            #+#    #+#             */
/*   Updated: 2025/02/10 15:19:42 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_export_string(char *environ_string, int out_fd)
{
	int	i;

	i = 0;
	ft_putstr_fd("declare -x ", out_fd);
	while (environ_string[i])
	{
		ft_putchar_fd(environ_string[i], out_fd);
		if (environ_string[i++] == '=')
		{
			ft_putchar_fd(DOUBLE_QUOTE, out_fd);
			while (environ_string[i])
			{
				if (environ_string[i] == DOUBLE_QUOTE)
					ft_putchar_fd('\\', out_fd);
				ft_putchar_fd(environ_string[i++], out_fd);
			}
			ft_putchar_fd(DOUBLE_QUOTE, out_fd);
		}
	}
	ft_putchar_fd('\n', out_fd);
}

void	append_var(t_msdata *msdata, const char *input)
{
	char	*var_name;
	char	*update_string;
	char	*current_value;
	size_t	total_len;

	var_name = ft_calloc(1, ft_strchr(input, '+') - input + 1);
	ft_strlcpy(var_name, input, ft_strchr(input, '+') - input + 1);
	current_value = get_env_value(var_name, msdata->env);
	total_len = ft_strchr(input, '+') - input + 1;
	if (current_value)
		total_len += ft_strlen(current_value);
	total_len += ft_strlen(ft_strchr(input, '=') + 1) + 1;
	update_string = ft_calloc(1, total_len);
	ft_strlcpy(update_string, var_name, ft_strlen(var_name) + 1);
	ft_strlcat(update_string, "=", ft_strlen(var_name) + 2);
	if (current_value)
		ft_strlcat(update_string, current_value, ft_strlen(update_string)
			+ ft_strlen(current_value) + 1);
	strcat(update_string, ft_strchr(input, '=') + 1);
	safe_free(&var_name);
	msdata->env = add_var_to_env(update_string, msdata->env);
	safe_free(&update_string);
}

bool	is_append(char *arg)
{
	return (ft_strchr(arg, '=') && *(ft_strchr(arg, '=') - 1) == '+');
}

int	export_variables(t_msdata *msdata, char **args)
{
	int	exit_status;
	int	i;

	i = 0;
	exit_status = 0;
	while (args[i])
	{
		if (!is_identifier_valid(args[i]))
		{
			ft_dprintf(STDERR_FILENO, MS_MES EXPORT_NOT_VALID_ID, args[i]);
			exit_status = 1;
		}
		else if (is_append(args[i]))
			append_var(msdata, args[i]);
		else
		{
			msdata->env = add_var_to_env(args[i], msdata->env);
			if (!(msdata->env))
				fatal_error(msdata, MALLOC_ERR, MALLOC_ERRCD);
		}
		i++;
	}
	return (exit_status);
}

int	ms_export(t_msdata *msdata, char **args)
{
	char	**sorted_environ;
	int		i;

	if (!args[0])
	{
		i = 0;
		sorted_environ = sort_array(copy_env(msdata, msdata->env));
		if (!sorted_environ)
			return (ft_putstr_fd("error in malloc\n", msdata->out_fd), 1);
		while (sorted_environ[i])
			print_export_string(sorted_environ[i++], msdata->out_fd);
		safe_free_array(&sorted_environ);
		return (0);
	}
	else
		return (export_variables(msdata, args));
	return (0);
}
