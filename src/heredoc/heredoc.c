/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 16:00:59 by npizzi            #+#    #+#             */
/*   Updated: 2025/01/30 13:05:11 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Creates temporary file to store heredoc information, handle signals
 *  and errors.
 */
void	heredoc_handler(char *delim, t_msdata *msdata, int tempfile_fd,
		bool delim_is_quoted)
{
	char	*line;
	int		delim_len;

	install_heredoc_signals();
	delim_len = ft_strlen(delim);
	while (true)
	{
		line = readline("> ");
		if (g_sig_received == SIGINT)
			return (safe_free(&line));
		if (!line)
		{
			ft_dprintf(msdata->out_fd, MS_MES WARN_MES EOF_WARN EOF_WANT,
				msdata->line_num, delim);
			return ;
		}
		if (ft_strncmp(line, delim, ft_max(ft_strlen(line), delim_len)) == 0)
			return (safe_free(&line));
		if (!delim_is_quoted)
			line = expand_quoted_var(line, msdata);
		ft_dprintf(tempfile_fd, "%s\n", line);
		safe_free(&line);
	}
}

/** @brief Replaces heredoc node with io_redirect where filename is the
 *  temporary file created for the heredoc
 */
void	change_heredoc_node(t_msdata *msdata, t_ast_node *heredoc_node,
		int tempfile_fd)
{
	safe_free(&(heredoc_node->value));
	heredoc_node->value = ft_strdup("<");
	if (!(heredoc_node->value))
		return ;
	heredoc_node->right->type = AST_FILENAME;
	safe_free(&(heredoc_node->right->value));
	heredoc_node->right->value = array_last(msdata->heredocs);
	close_safe(tempfile_fd);
}

void	fork_and_wait(t_ast_node *heredoc_node, t_msdata *msdata)
{
	int		tempfile_fd;
	int		exit_status;
	pid_t	pid;

	generate_temp_file(msdata);
	tempfile_fd = open(array_last(msdata->heredocs), O_CREAT | O_RDWR, 0600);
	if (tempfile_fd == -1)
		return ;
	pid = fork();
	if (pid == CHILD_PROCESS_ID)
	{
		free(array_last(msdata->heredocs));
		heredoc_handler(heredoc_node->right->value, msdata, tempfile_fd,
			heredoc_node->is_quoted);
		free_msdata(msdata);
		rl_clear_history();
		close_safe(tempfile_fd);
		if (g_sig_received == SIGINT)
			exit(130);
		exit(EXIT_SUCCESS);
	}
	waitpid(pid, &exit_status, 0);
	if (WIFEXITED(exit_status))
		msdata->last_exit = WEXITSTATUS(exit_status);
	change_heredoc_node(msdata, heredoc_node, tempfile_fd);
}

int	check_for_heredocs(t_ast_node *node, t_msdata *msdata)
{
	if (!node)
		return (0);
	if (node->type == AST_PIPELINE)
	{
		if (check_for_heredocs(node->left, msdata) != 0)
			return (1);
		if (check_for_heredocs(node->right, msdata) != 0)
			return (1);
	}
	if (node->type == AST_COMMAND)
		node = node->left;
	while (node)
	{
		if (node->type == AST_IO_REDIRECT && node->value[1] == '<')
		{
			fork_and_wait(node, msdata);
			if (msdata->last_exit != 0)
				return (1);
		}
		node = node->right;
	}
	return (0);
}
