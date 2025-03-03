/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 12:40:06 by thopgood          #+#    #+#             */
/*   Updated: 2025/02/10 14:50:17 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_pwd(t_msdata *msdata)
{
	char	*working_directory;
	char	*pwd_variable;

	working_directory = getcwd(NULL, 0);
	if (!working_directory)
	{
		ft_putstr_fd("Error getting current dir\n", STDERR_FILENO);
		return (1);
	}
	pwd_variable = ft_strjoin("PWD=", working_directory);
	if (!pwd_variable)
		return (free(working_directory), 1);
	msdata->env = add_var_to_env(pwd_variable, msdata->env);
	safe_free(&pwd_variable);
	safe_free(&working_directory);
	if (!msdata->env)
		return (1);
	return (0);
}

int	ms_pwd(t_msdata *msdata)
{
	char	*working_directory;

	working_directory = getcwd(NULL, 0);
	if (!working_directory)
	{
		ft_putstr_fd(MS_MES PWD_ERR, STDERR_FILENO);
		return (1);
	}
	ft_putstr_fd(working_directory, msdata->out_fd);
	ft_putchar_fd('\n', msdata->out_fd);
	safe_free(&working_directory);
	return (0);
}
