/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 17:23:35 by npizzi            #+#    #+#             */
/*   Updated: 2025/01/30 14:42:08 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Stores received signal number in global g_sig_received to allow
 *  default behaviour to be overridden.
 */
void	signal_handler(int signum)
{
	g_sig_received = signum;
}

/** @brief Handles SIGINT (ctrl+C) but ignores SIGQUIT (ctrl+\)
 */
void	install_default_handler(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGPIPE, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}
