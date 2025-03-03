/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cli_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 17:18:03 by npizzi            #+#    #+#             */
/*   Updated: 2025/02/10 16:00:21 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_the_shell(t_msdata *msdata)
{
	int	exit_status;

	if (msdata->is_interactive)
		ft_dprintf(STDOUT_FILENO, "exit\n");
	exit_status = msdata->last_exit;
	free_msdata(msdata);
	rl_clear_history();
	exit(exit_status);
}

/** @brief If no input -> ctrl+D has been received
 */
void	process_user_input(t_msdata *msdata)
{
	int	exit_status;

	if (!(msdata->input))
		exit_the_shell(msdata);
	signal(SIGINT, SIG_IGN);
	if (msdata->input[0] == 0)
		return (safe_free(&(msdata->input)));
	if (msdata->is_interactive)
		add_history(msdata->input);
	tokenise(msdata);
	if (!msdata->token_list)
	{
		if (msdata->has_input)
			msdata->last_exit = 0;
		return ;
	}
	msdata->ast = parse(msdata, msdata->token_list);
	if (msdata->ast && !check_for_heredocs(msdata->ast, msdata))
		traverse_tree(msdata->ast, msdata);
	wait_and_update_exit(-1, &exit_status, msdata);
	unlink_all_heredocs(msdata);
	free_ast(msdata->ast);
	free_list(&msdata->token_list);
	msdata->ast = NULL;
}
