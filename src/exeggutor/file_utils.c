/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 10:42:35 by npizzi            #+#    #+#             */
/*   Updated: 2025/01/27 20:02:10 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief duplicates FDs
 */
void	dup2_io(t_msdata *msdata)
{
	int	result;

	if (msdata->in_fd != STDIN_FILENO)
	{
		result = dup2(msdata->in_fd, STDIN_FILENO);
		if (result == -1)
			ft_dprintf(STDERR_FILENO, "error duplicating infile - %d %s\n ",
				msdata->in_fd, strerror(errno));
		close_safe(msdata->in_fd);
		msdata->in_fd = STDIN_FILENO;
	}
	if (msdata->out_fd != STDOUT_FILENO)
	{
		result = dup2(msdata->out_fd, STDOUT_FILENO);
		if (result == -1)
			ft_dprintf(STDERR_FILENO, "error duplicating outfile - %d %s\n",
				msdata->out_fd, strerror(errno));
		close_safe(msdata->out_fd);
		msdata->out_fd = STDOUT_FILENO;
	}
	if (msdata->is_pipe)
	{
		close_safe(msdata->pipe_fd[READ_END]);
		close_safe(msdata->pipe_fd[WRITE_END]);
	}
}

/** @brief Closes FD if open and not connected to terminal. Sets FD to -1.
 */
void	close_safe(int fd)
{
	if (fd == STDIN_FILENO || fd == STDOUT_FILENO)
	{
		return ;
	}
	if (fd != FD_CLOSED && !isatty(fd))
		close(fd);
	fd = FD_CLOSED;
}

int	redirect_input(t_ast_node *redirection_node, t_msdata *msdata)
{
	char	*filename;

	filename = redirection_node->right->value;
	if (ft_strlen(filename) > FNAME_MAXLEN)
	{
		ft_dprintf(STDERR_FILENO, MS_MES FNAME_LEN_ERR);
		return (1);
	}
	close_safe(msdata->in_fd);
	msdata->in_fd = open(filename, O_RDONLY);
	if (msdata->in_fd == -1)
	{
		ft_dprintf(STDERR_FILENO, MS_MES "%s: %s\n", filename, strerror(errno));
		return (1);
	}
	return (0);
}

int	redirect_output(t_ast_node *redirection_node, t_msdata *msdata)
{
	bool	truncate;
	char	*filename;

	filename = redirection_node->right->value;
	if (ft_strlen(filename) > FNAME_MAXLEN)
	{
		ft_dprintf(STDERR_FILENO, MS_MES FNAME_LEN_ERR, filename);
		return (1);
	}
	truncate = true;
	close_safe(msdata->out_fd);
	if ((redirection_node->value[1]) == '>')
		truncate = false;
	if (truncate)
		msdata->out_fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
	else
		msdata->out_fd = open(filename, O_RDWR | O_CREAT | O_APPEND, 0644);
	if (msdata->out_fd == -1)
	{
		ft_dprintf(STDERR_FILENO, MS_MES "%s: %s\n", filename, strerror(errno));
		return (1);
	}
	return (0);
}
