/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exeggutor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 18:19:17 by npizzi            #+#    #+#             */
/*   Updated: 2025/02/10 14:54:24 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	try_command(char *path, char **argv, t_msdata *msdata)
{
	if (!path)
		return (MALLOC_ERRCD);
	if (access(path, F_OK) == 0)
	{
		if (access(path, X_OK) == 0)
			execve(path, argv, msdata->env);
		print_err(join_var(5, MS_MES, argv[0], ": ", strerror(errno), NL));
		safe_free(&path);
		return (126);
	}
	safe_free(&path);
	return (0);
}

int	execute_simple_command(t_msdata *msdata, char **argv, char **paths)
{
	int		i;
	int		exit_status;
	char	*command;

	exit_status = 0;
	if (!argv[0][0])
	{
		print_err(join_var(3, MS_MES, argv[0], NOCMD_ERR));
		return (127);
	}
	command = ft_strjoin("/", argv[0]);
	if (!command)
		return (MALLOC_ERRCD);
	i = -1;
	while (paths[++i] && !exit_status)
		exit_status = try_command(ft_strjoin(paths[i], command), argv, msdata);
	safe_free(&command);
	if (!exit_status)
	{
		print_err(join_var(3, MS_MES, argv[0], NOCMD_ERR));
		return (127);
	}
	return (exit_status);
}

/** @brief Installs signal handlers, handles empty command and checks if file
 *  is a directory. Executes non-built-in and non-pipeline commands.
 *  @returns 126: command is directory. 127: command is empty.
 */
int	execute_non_builtin(t_msdata *msdata, char **argv)
{
	int		exit_status;
	char	**paths;

	paths = ft_split(get_env_value("PATH", msdata->env), ':');
	if (ft_strchr(argv[0], '/') && is_directory(argv[0]))
	{
		print_err(join_var(3, MS_MES, argv[0], IS_DIR_ERR));
		exit_status = 126;
	}
	else if (ft_strchr(argv[0], '/') || !(paths) || !(paths[0]))
	{
		if (access(argv[0], X_OK) == 0)
			execve(argv[0], argv, msdata->env);
		print_err(join_var(5, MS_MES, argv[0], ": ", strerror(errno), NL));
		exit_status = 126 - access(argv[0], F_OK);
	}
	else
		exit_status = execute_simple_command(msdata, argv, paths);
	safe_free_array(&paths);
	return (exit_status);
}

/** @brief Determines whether command is built-in or not then executes
 *  accordingly.
 */
void	execute_command(t_msdata *msdata, char **argv)
{
	if (ft_strncmp(argv[0], "cd", 3) == 0)
		msdata->last_exit = ms_cd(msdata, argv + 1);
	else if (ft_strncmp(argv[0], "echo", 5) == 0)
		msdata->last_exit = ms_echo(msdata, argv + 1);
	else if (ft_strncmp(argv[0], "env", 4) == 0)
		msdata->last_exit = ms_env(msdata);
	else if (ft_strncmp(argv[0], "exit", 5) == 0)
	{
		if (!msdata->is_pipe && msdata->is_interactive)
			ft_dprintf(msdata->out_fd, "exit\n");
		ms_exit(msdata, argv);
	}
	else if (ft_strncmp(argv[0], "export", 7) == 0)
		msdata->last_exit = ms_export(msdata, argv + 1);
	else if (ft_strncmp(argv[0], "pwd", 4) == 0)
		msdata->last_exit = ms_pwd(msdata);
	else if (ft_strncmp(argv[0], "unset", 6) == 0)
		msdata->last_exit = ms_unset(msdata->env, argv + 1);
	else
		msdata->last_exit = execute_non_builtin(msdata, argv);
}
