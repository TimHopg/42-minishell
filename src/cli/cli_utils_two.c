/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cli_utils_two.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:55:35 by npizzi            #+#    #+#             */
/*   Updated: 2025/02/10 15:33:13 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief builds prompt based on existence of USER var or not
 */
int	get_prompt(t_msdata *msdata)
{
	char	*user;

	if (msdata->prompt)
		safe_free(&msdata->prompt);
	user = get_env_value("USER", msdata->env);
	if (user && user[0])
		msdata->prompt = ft_strjoin(user, "@minishell> ");
	else
		msdata->prompt = ft_strdup("minishell> ");
	if (!msdata->prompt)
		fatal_error(msdata, MALLOC_ERR, MALLOC_ERRCD);
	return (0);
}

/** @brief handles behaviour when CTRL+C is received
 */
int	ft_event_hook(void)
{
	if (g_sig_received == SIGINT)
	{
		ft_dprintf(STDOUT_FILENO, "\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_sig_received = 130;
	}
	return (0);
}

void	free_msdata(t_msdata *msdata)
{
	safe_free(&(msdata->prompt));
	safe_free(&(msdata->input));
	safe_free_array(&msdata->env);
	free_ast(msdata->ast);
	free_list(&msdata->token_list);
	if (msdata->is_pipe)
	{
		close_safe(msdata->pipe_fd[READ_END]);
		close_safe(msdata->pipe_fd[WRITE_END]);
	}
	close_safe(msdata->in_fd);
	close_safe(msdata->out_fd);
}

/** @brief Resets default values of ms struct
 */
void	reset_default_values(t_msdata *msdata)
{
	int	i;

	i = -1;
	while (++i < MAX_HEREDOCS_NUM)
		msdata->heredocs[i] = NULL;
	close_safe(msdata->in_fd);
	msdata->in_fd = STDIN_FILENO;
	close_safe(msdata->out_fd);
	msdata->out_fd = STDOUT_FILENO;
	msdata->previous_fd = STDIN_FILENO;
	msdata->is_pipe = false;
	msdata->is_last = false;
	msdata->has_input = false;
	install_default_handler();
	rl_signal_event_hook = &ft_event_hook;
}
