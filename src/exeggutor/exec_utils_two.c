/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_two.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:40:05 by npizzi            #+#    #+#             */
/*   Updated: 2025/01/27 20:01:20 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_directory(char *path)
{
	struct stat	buffer;

	if (!path || stat(path, &buffer) != 0)
		return (false);
	return (S_ISDIR(buffer.st_mode));
}

/** @brief Fork only if is a pipe or the command is not builtin
 */
bool	has_to_be_forked(t_ast_node *command_node, t_msdata *msdata)
{
	char	*cmd_name;

	if (msdata->is_pipe)
		return (true);
	if (command_node->type == AST_CMD_NAME)
	{
		cmd_name = command_node->value;
		if (ft_strncmp(cmd_name, "cd", 3) == 0)
			return (false);
		if (ft_strncmp(cmd_name, "echo", 5) == 0)
			return (false);
		if (ft_strncmp(cmd_name, "env", 4) == 0)
			return (false);
		if (ft_strncmp(cmd_name, "exit", 5) == 0)
			return (false);
		if (ft_strncmp(cmd_name, "export", 7) == 0)
			return (false);
		if (ft_strncmp(cmd_name, "pwd", 4) == 0)
			return (false);
		if (ft_strncmp(cmd_name, "unset", 6) == 0)
			return (false);
		return (true);
	}
	return (false);
}
