/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_one.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 17:44:10 by npizzi            #+#    #+#             */
/*   Updated: 2025/02/10 14:58:32 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Creates a temporary file in tmp folder to handle heredoc.
 *  @returns A string with the name of the file.
 */
void	generate_temp_file(t_msdata *msdata)
{
	static int	temp_file_num = 0;
	char		*file_name;
	char		*file_num;
	int			i;

	file_num = ft_itoa(temp_file_num);
	if (!file_num)
		return ;
	file_name = ft_strjoin("/tmp/ms_heredoc_", file_num);
	free(file_num);
	if (!file_name)
		return ;
	if (access(file_name, F_OK) == 0)
	{
		safe_free(&file_name);
		temp_file_num++;
		return (generate_temp_file(msdata));
	}
	i = 0;
	while (msdata->heredocs[i])
		i++;
	msdata->heredocs[i] = file_name;
}

void	unlink_all_heredocs(t_msdata *msdata)
{
	int	i;

	i = 0;
	while (msdata->heredocs[i])
	{
		unlink(msdata->heredocs[i]);
		i++;
	}
}

/** @brief Fetches the last value from array of strings
 */
char	*array_last(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return (NULL);
	while (array[i + 1])
		i++;
	return (array[i]);
}
