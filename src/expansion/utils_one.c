/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_one.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 17:43:08 by thopgood          #+#    #+#             */
/*   Updated: 2025/02/10 14:56:30 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * If array exists, frees all elements in it and then the pointer
 * to the array. Sets the pointer to the array to NULL.
 */
void	safe_free_array(char ***arr)
{
	int	i;

	if (!arr || !*arr)
		return ;
	i = 0;
	while ((*arr)[i])
	{
		safe_free(&(*arr)[i]);
		i++;
	}
	free(*arr);
	*arr = NULL;
}

/*
 * Removes (in place) first and last char if first char is ' or "
 */
void	remove_quotes_in_place(char **str)
{
	int		str_len;
	char	*temp_str;

	if ((*str)[0] == SINGLE_QUOTE || (*str)[0] == DOUBLE_QUOTE)
	{
		str_len = ft_strlen(*str);
		temp_str = *str;
		*str = ft_substr(*str, 1, str_len - 2);
		safe_free(&temp_str);
	}
}

void	remove_token_from_list(t_token_node *curr_tok, t_token_node *next_tok)
{
	if (curr_tok->combine_next == true)
		curr_tok->combine_next = next_tok->combine_next;
	curr_tok->next = next_tok->next;
	safe_free(&next_tok->value);
	free(next_tok);
}

/** @brief Traverses token list and updates token type to WORD if variable token
 * 	is encountered. If result of expansion is empty value, node is deleted from
 * 	the list
 */
void	update_tokens_list(t_token_node **head_of_list)
{
	t_token_node	*curr;
	t_token_node	*next_tok;

	while (*(head_of_list) && (*head_of_list)->type == TOK_ENV_VAR
		&& !(*head_of_list)->value[0])
	{
		curr = *head_of_list;
		*head_of_list = (*head_of_list)->next;
		safe_free(&curr->value);
		free(curr);
	}
	if (!*head_of_list)
		return ;
	curr = *head_of_list;
	while (curr)
	{
		next_tok = curr->next;
		if (curr->type == TOK_ENV_VAR || curr->type == TOK_DBL_QT
			|| curr->type == TOK_SNGL_QT)
			curr->type = TOK_WORD;
		if (next_tok && next_tok->type == TOK_ENV_VAR && !(next_tok->value[0]))
			remove_token_from_list(curr, next_tok);
		curr = curr->next;
	}
}
