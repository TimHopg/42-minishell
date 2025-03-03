/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 12:16:06 by thopgood          #+#    #+#             */
/*   Updated: 2025/01/27 20:19:34 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Recursively frees AST
 */
void	free_ast(t_ast_node *node)
{
	if (!node)
		return ;
	free_ast(node->left);
	node->left = NULL;
	free_ast(node->right);
	node->right = NULL;
	if (node->value)
	{
		free(node->value);
		node->value = NULL;
	}
	free(node);
	node = NULL;
}

/** @brief Creates AST node using safe_calloc and handles malloc error
 */
t_ast_node	*create_ast_node(t_msdata *msdata, t_ast_type type, char *value)
{
	t_ast_node	*node;

	node = safe_calloc(msdata, sizeof(t_ast_node));
	node->type = type;
	if (value && value[0])
		node->value = ft_strdup(value);
	else
		node->value = ft_strdup("");
	node->left = NULL;
	node->right = NULL;
	node->is_quoted = false;
	return (node);
}

/** @brief Consumes a token and advances list (returns the next node)
 */
t_token_node	*consume_token(t_token_node **tokens)
{
	t_token_node	*current;

	if (!*tokens)
		return (NULL);
	current = *tokens;
	*tokens = current->next;
	return (current);
}

/** @brief Is tok_type '<', '<<', '>' or '>>'?
 */
int	is_redir(t_token_type tok_type)
{
	return (TOK_REDIR_IN <= tok_type && tok_type <= TOK_HEREDOC);
}
