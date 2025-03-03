/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 12:19:30 by thopgood          #+#    #+#             */
/*   Updated: 2025/01/27 20:19:31 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Parse an IO redirection ->
 *  io_redirect: io_file | io_here
 */
t_ast_node	*parse_io_redirect(t_msdata *msdata, t_token_node **tokens)
{
	t_token_node	*token;
	t_ast_node		*redirect;
	t_token_node	*file_token;
	t_ast_node		*filename;

	token = consume_token(tokens);
	redirect = create_ast_node(msdata, AST_IO_REDIRECT, token->value);
	if (token->type == TOK_HEREDOC)
		redirect->is_quoted = token->is_quoted;
	file_token = consume_token(tokens);
	filename = create_ast_node(msdata, AST_FILENAME, file_token->value);
	redirect->right = filename;
	return (redirect);
}

/** @brief Parse here document ->
 *  io_here: HEREDOC here_end
 */
t_ast_node	*parse_heredoc(t_msdata *msdata, t_token_node **tokens)
{
	t_token_node	*token;
	t_ast_node		*heredoc;
	t_token_node	*delimiter_token;
	t_ast_node		*delimiter;

	token = consume_token(tokens);
	heredoc = create_ast_node(msdata, AST_HEREDOC, token->value);
	delimiter_token = consume_token(tokens);
	delimiter = create_ast_node(msdata, AST_HEREEND, delimiter_token->value);
	heredoc->right = delimiter;
	return (heredoc);
}
