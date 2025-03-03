/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils_one.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 13:28:07 by thopgood          #+#    #+#             */
/*   Updated: 2025/01/27 20:22:55 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_list(t_token_node **lst)
{
	t_token_node	*curr;
	t_token_node	*temp;

	if (!lst || !*lst)
		return ;
	curr = *lst;
	while (curr)
	{
		temp = curr->next;
		safe_free(&(curr->value));
		free(curr);
		curr = temp;
	}
	*lst = NULL;
}

t_token_node	*new_token_node(char *value, t_token_type type,
		t_msdata *msdata)
{
	t_token_node	*node;

	if (!value)
		fatal_error(msdata, MALLOC_ERR, MALLOC_ERRCD);
	node = malloc(sizeof(t_token_node));
	if (!node)
	{
		safe_free(&value);
		fatal_error(msdata, MALLOC_ERR, MALLOC_ERRCD);
	}
	node->value = value;
	node->type = type;
	node->next = NULL;
	node->combine_next = false;
	node->is_quoted = false;
	return (node);
}

t_token_node	*lstlast(t_token_node *lst)
{
	t_token_node	*node;

	if (lst == NULL)
		return (NULL);
	node = lst;
	while (node->next != NULL)
		node = node->next;
	return (node);
}

void	lst_append(t_token_node **lst, t_token_node *new)
{
	t_token_node	*curr;

	if (lst == NULL && new == NULL)
		return ;
	curr = *lst;
	if (*lst == NULL)
		*lst = new;
	else
		lstlast(curr)->next = new;
}
