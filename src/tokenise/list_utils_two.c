/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils_two.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 10:21:46 by npizzi            #+#    #+#             */
/*   Updated: 2025/01/27 20:23:00 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	token_list_size(t_token_node *token_head)
{
	int				list_size;
	t_token_node	*ptr;

	list_size = 0;
	ptr = token_head;
	while (ptr)
	{
		ptr = ptr->next;
		list_size++;
	}
	return (list_size);
}

void	safe_free(char **ptr)
{
	if (*ptr)
		free(*ptr);
	*ptr = NULL;
}
