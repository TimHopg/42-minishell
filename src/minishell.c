/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 16:20:59 by npizzi            #+#    #+#             */
/*   Updated: 2025/02/10 16:03:21 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Global variable stores number of signal received
 */
sig_atomic_t	g_sig_received;

int	ms_noninteractive(t_msdata *msdata)
{
	int	exit_status;

	while (get_next_line(STDIN_FILENO, &msdata->input))
	{
		reset_default_values(msdata);
		process_user_input(msdata);
		safe_free(&msdata->input);
	}
	exit_status = msdata->last_exit;
	free_msdata(msdata);
	return (exit_status);
}

/** @brief Initialises msdata struct, copies environment, sets prompt,
sets in and out FDs.
 */
int	init_shell(t_msdata *msdata, char *envp[])
{
	ft_bzero(msdata, sizeof(t_msdata));
	msdata->env = copy_env_if(msdata, envp);
	if (!envp[0])
	{
		update_pwd(msdata);
		msdata->env = add_var_to_env("OLDPWD", msdata->env);
		msdata->env = add_var_to_env(DEFAULT_PATH, msdata->env);
	}
	if (get_prompt(msdata) != 0)
		return (safe_free_array(&msdata->env), EXIT_FAILURE);
	update_shell_level(msdata);
	msdata->out_fd = STDOUT_FILENO;
	return (0);
}

/** @brief Minishell. Performs initial checks then loops for user input
and executes until termination.
 */
int	main(int argc, char *argv[], char *envp[])
{
	static t_msdata	msdata;

	(void)argv;
	if (argc != 1)
		fatal_error(&msdata, ARG_ERR, ARG_ERRCD);
	if (init_shell(&msdata, envp) != 0)
		fatal_error(&msdata, INIT_ERR, INIT_ERRCD);
	msdata.is_interactive = (isatty(STDIN_FILENO));
	if (!msdata.is_interactive)
		return (ms_noninteractive(&msdata));
	while (true)
	{
		reset_default_values(&msdata);
		msdata.input = readline(msdata.prompt);
		if (g_sig_received == 130)
		{
			msdata.last_exit = 130;
			g_sig_received = 0;
		}
		msdata.line_num += 1;
		process_user_input(&msdata);
		safe_free(&msdata.input);
	}
	return (0);
}
