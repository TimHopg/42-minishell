/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_two.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 14:42:50 by npizzi            #+#    #+#             */
/*   Updated: 2025/01/27 20:15:11 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_heredocs(t_token_node *tokens_list)
{
	int				heredocs_num;
	t_token_node	*ptr;

	heredocs_num = 0;
	ptr = tokens_list;
	while (ptr)
	{
		if (ptr->type == TOK_HEREDOC)
			heredocs_num++;
		ptr = ptr->next;
	}
	return (heredocs_num);
}

void	update_values(t_token_node *token, t_token_node *heredoc)
{
	if (token->type == TOK_SNGL_QT || token->type == TOK_DBL_QT)
	{
		if (ft_strlen(token->value) > 2)
			heredoc->is_quoted = true;
	}
	if (!(token->next) || !token->combine_next)
	{
		if (token->type >= TOK_ENV_VAR && token->type <= TOK_DBL_QT)
			token->type = TOK_SNGL_QT;
	}
	else if (token->combine_next)
		token->type = TOK_SNGL_QT;
}

/** @brief Transforms heredoc delimter tokens into a single quote token.
 * 	This is to prevent variables of the heredoc from being expanded and to make
 * 	sure all quotes are removed correctly.
 */
void	adjust_heredoc_delimiter(t_token_node *tokens)
{
	t_token_node	*heredoc_node;

	while (tokens)
	{
		if (tokens->type == TOK_HEREDOC)
		{
			heredoc_node = tokens;
			tokens = tokens->next;
			while (tokens)
			{
				update_values(tokens, heredoc_node);
				if (!(tokens->next) || !tokens->combine_next)
					break ;
				tokens = tokens->next;
			}
		}
		else
			tokens = tokens->next;
	}
}
