/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 11:08:43 by npizzi            #+#    #+#             */
/*   Updated: 2025/01/30 15:46:16 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Waits for all processes to finish, prints only one message and 
 * 	updates the exit status to match that of the last child created.
 */
void	wait_and_update_exit(pid_t pid, int *exit_status, t_msdata *msdata)
{
	pid_t	returning_pid;
	int		sig_exited;

	returning_pid = waitpid(pid, exit_status, 0);
	sig_exited = 0;
	while (returning_pid != -1)
	{
		if (WIFSIGNALED(*exit_status))
		{
			*exit_status = WTERMSIG(*exit_status);
			if (*exit_status == 2 && !sig_exited)
				ft_dprintf(STDOUT_FILENO, "\n");
			else if (*exit_status == 3 && !sig_exited)
				ft_dprintf(STDOUT_FILENO, "Quit (core dumped)\n");
			if (returning_pid == msdata->last_pid)
				msdata->last_exit = *exit_status + 128;
			sig_exited++;
		}
		else if (WIFEXITED(*exit_status) && returning_pid == msdata->last_pid)
			msdata->last_exit = WEXITSTATUS(*exit_status);
		returning_pid = waitpid(pid, exit_status, 0);
	}
}

int	argv_size(t_ast_node *command_node)
{
	t_ast_node	*ptr;
	int			argv_size;

	if (!command_node)
		return (0);
	argv_size = 1;
	ptr = command_node->right;
	while (ptr)
	{
		if (ptr->type == AST_ARG)
			argv_size++;
		ptr = ptr->right;
	}
	return (argv_size);
}

/** @brief Creates argv array, in position 0 is the name of the command.
 * 	it does not copy i/o redirections nor heredocs
 */
char	**create_argv(t_ast_node *command_node)
{
	char		**argv;
	t_ast_node	*ptr;
	int			i;

	argv = malloc((argv_size(command_node) + 1) * sizeof(char *));
	if (!argv)
		return (NULL);
	i = 0;
	ptr = command_node;
	while (ptr)
	{
		if (ptr->type == AST_ARG || ptr->type == AST_CMD_NAME)
		{
			argv[i] = ft_strdup(ptr->value);
			if (!argv[i])
				return (safe_free_array(&argv), NULL);
			i++;
		}
		ptr = ptr->right;
	}
	argv[i] = NULL;
	return (argv);
}
