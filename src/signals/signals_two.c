/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_two.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 19:29:08 by npizzi            #+#    #+#             */
/*   Updated: 2025/02/10 17:16:34 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_sigint_handler(int signum)
{
	if (signum == SIGINT)
		g_sig_received = signum;
}

int	ft_readline_heredoc_hook(void)
{
	if (g_sig_received == SIGINT)
		rl_done = 1;
	return (0);
}

void	install_heredoc_signals(void)
{
	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	rl_event_hook = &ft_readline_heredoc_hook;
}

/** @brief Children inherit signal behaviour changes so are reset to default
 * 	beahviour.
 */
void	install_children_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
