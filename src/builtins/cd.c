/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 13:20:13 by thopgood          #+#    #+#             */
/*   Updated: 2025/02/10 14:49:43 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_if_valid(char *path)
{
	if (ft_strlen(path) >= FNAME_MAXLEN)
	{
		ft_dprintf(STDERR_FILENO, MS_MES "cd: " FNAME_LEN_ERR, path);
		return (1);
	}
	if (access(path, F_OK) != 0)
	{
		ft_dprintf(STDERR_FILENO, MS_MES "cd: %s: No such file or directory\n",
			path);
		return (1);
	}
	if (!is_directory(path))
	{
		ft_dprintf(STDERR_FILENO, MS_MES "cd: %s: Not a directory\n", path);
		return (1);
	}
	return (0);
}

/** @brief Determines target directory for cd command.
 *  Handles '-' using OLDPWD and '~' and handles errors accordingly.
 */
char	*get_target_path(char *path, char *home, t_msdata *msdata)
{
	char	*oldpwd;

	if (path && path[0] == 0)
		return (path);
	if (!path || ft_strncmp(path, "~", ft_strlen(path)) == 0)
	{
		if (home == NULL)
			ft_putstr_fd(MS_MES "cd: HOME not set\n", STDERR_FILENO);
		return (home);
	}
	if (ft_strncmp(path, "-", ft_strlen(path)) == 0)
	{
		oldpwd = get_env_value("OLDPWD", msdata->env);
		if (oldpwd && oldpwd[0])
		{
			if (!access(oldpwd, F_OK | W_OK))
				ft_dprintf(msdata->out_fd, "%s\n", oldpwd);
			return (oldpwd);
		}
		ft_putstr_fd(MS_MES "cd: OLDPWD not set\n", STDERR_FILENO);
		return (NULL);
	}
	return (path);
}

/** @brief Changes current directory to target path and updates env vars
 */
static int	change_directory(char *path, char *oldpwd, t_msdata *msdata)
{
	char	*oldpwd_var;

	if (check_if_valid(path) == 1)
		return (1);
	if (chdir(path) < 0)
	{
		ft_dprintf(STDERR_FILENO, MS_MES "cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	if (oldpwd)
		oldpwd_var = ft_strjoin("OLDPWD=", oldpwd);
	else
		oldpwd_var = ft_strdup("OLDPWD");
	if (!oldpwd_var)
		fatal_error(msdata, MALLOC_ERR, MALLOC_ERRCD);
	if (update_var_value_overwrite(oldpwd_var, msdata->env) != 0)
		msdata->env = add_var_to_env(oldpwd_var, msdata->env);
	safe_free(&oldpwd_var);
	if (!msdata->env)
		fatal_error(msdata, MALLOC_ERR, MALLOC_ERRCD);
	if (update_pwd(msdata) != 0)
		return (2);
	return (0);
}

/** @brief Built-in command cd: navigates to given path
 */
int	ms_cd(t_msdata *msdata, char **args)
{
	char	*home;
	char	*oldpwd;
	char	*path;
	int		exit_status;

	if (args[0] && args[1])
	{
		ft_putstr_fd(MS_MES CDARGS_ERR, STDERR_FILENO);
		return (1);
	}
	oldpwd = get_env_value("PWD", msdata->env);
	home = get_env_value("HOME", msdata->env);
	path = get_target_path(args[0], home, msdata);
	if (!path)
		return (1);
	if (path[0] == 0)
		path = oldpwd;
	exit_status = change_directory(path, oldpwd, msdata);
	return (exit_status);
}
