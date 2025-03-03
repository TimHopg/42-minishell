/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_splitter.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 17:45:36 by npizzi            #+#    #+#             */
/*   Updated: 2025/01/31 12:38:08 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipeline(t_ast_node *node, t_msdata *msdata)
{
	msdata->is_pipe = true;
	if (pipe(msdata->pipe_fd) != 0)
		ft_dprintf(STDERR_FILENO, "pipe: %s\n", strerror(errno));
	msdata->out_fd = msdata->pipe_fd[WRITE_END];
	traverse_tree(node->left, msdata);
	close_safe(msdata->pipe_fd[WRITE_END]);
	close_safe(msdata->previous_fd);
	msdata->previous_fd = msdata->pipe_fd[READ_END];
	msdata->in_fd = msdata->previous_fd;
	if (!node->right->right)
	{
		msdata->is_last = true;
		close_safe(msdata->out_fd);
		msdata->out_fd = STDOUT_FILENO;
	}
	traverse_tree(node->right, msdata);
	close_safe(msdata->pipe_fd[READ_END]);
}

/** @brief Traverses command args and redirects input/output if redir is 
 * 	encountered.
 */
int	handle_redirections(t_ast_node *cmd_args, t_msdata *msdata)
{
	int	exit_status;

	exit_status = 0;
	while (cmd_args)
	{
		if (cmd_args->type == AST_IO_REDIRECT)
		{
			if (cmd_args->value[0] == '<')
				exit_status = redirect_input(cmd_args, msdata);
			else if (cmd_args->value[0] == '>')
				exit_status = redirect_output(cmd_args, msdata);
			if (exit_status != 0)
				break ;
		}
		cmd_args = cmd_args->right;
	}
	return (exit_status);
}

void	handle_command(t_ast_node *node, t_msdata *msdata)
{
	char	**argv;
	int		exit_status;
	bool	is_forked;

	is_forked = (msdata->is_pipe || has_to_be_forked(node, msdata));
	argv = create_argv(node);
	if (handle_redirections(node->right, msdata) == 0)
	{
		if (is_forked)
			dup2_io(msdata);
		execute_command(msdata, argv);
	}
	else
		msdata->last_exit = 1;
	safe_free_array(&argv);
	if (is_forked)
	{
		exit_status = msdata->last_exit;
		free_msdata(msdata);
		rl_clear_history();
		exit(exit_status);
	}
}

void	handle_command_node(t_ast_node *node, t_msdata *msdata)
{
	pid_t	pid;
	int		exit_status;

	if (has_to_be_forked(node->left, msdata))
	{
		pid = fork();
		if (pid < 0)
			return (ft_putstr_fd("Fork error\n", STDERR_FILENO));
		if (!msdata->is_pipe || msdata->is_last)
			msdata->last_pid = pid;
		if (pid == CHILD_PROCESS_ID)
		{
			install_children_signals();
			if (node->left->type == AST_IO_REDIRECT)
			{
				exit_status = handle_redirections(node->left, msdata);
				free_msdata(msdata);
				exit(exit_status);
			}
			else
				traverse_tree(node->left, msdata);
		}
	}
	else
		traverse_tree(node->left, msdata);
}

void	traverse_tree(t_ast_node *head_of_tree, t_msdata *msdata)
{
	if (!head_of_tree)
		return ;
	if (head_of_tree->type == AST_PIPELINE)
		handle_pipeline(head_of_tree, msdata);
	else if (head_of_tree->type == AST_COMMAND)
		handle_command_node(head_of_tree, msdata);
	else if (head_of_tree->type == AST_CMD_NAME)
		handle_command(head_of_tree, msdata);
	else if (head_of_tree->type == AST_IO_REDIRECT)
		msdata->last_exit = handle_redirections(head_of_tree, msdata);
}
