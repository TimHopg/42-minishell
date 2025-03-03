/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 19:17:58 by thopgood          #+#    #+#             */
/*   Updated: 2025/01/30 13:16:13 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_unsupported_token(t_token_type tok_type)
{
	if (TOK_AND <= tok_type && tok_type <= TOK_ENV_VAR)
		return (1);
	if (TOK_SNGL_QT <= tok_type && tok_type <= TOK_UNKNOWN)
		return (1);
	return (0);
}

/** @brief Checks that pipe isn't final token or followed by another pipe
 */
static int	is_pipe_error(t_token_node *curr)
{
	if (!curr->next)
		return (ft_dprintf(STDERR_FILENO, MS_MES SYN_ERR, NL_MES), 1);
	else if (curr->next->type == TOK_PIPE)
		return (ft_dprintf(STDERR_FILENO, MS_MES SYN_ERR, curr->next->value),
			1);
	return (0);
}

/** @brief Checks redir isn't final token and is followed by word
 */
static int	is_redir_error(t_token_node *curr)
{
	if (!curr->next)
		return (ft_dprintf(STDERR_FILENO, MS_MES SYN_ERR, NL_MES), 1);
	else if (curr->next->type != TOK_WORD)
		return (ft_dprintf(STDERR_FILENO, MS_MES SYN_ERR, curr->next->value),
			1);
	return (0);
}

/** @brief Prelimiary checks for lexicographical sense.
 */
int	token_checks(t_token_node *tokens)
{
	t_token_node	*curr;

	if (!tokens)
		return (1);
	curr = tokens;
	if (curr->type == TOK_PIPE)
		return (ft_dprintf(STDERR_FILENO, MS_MES SYN_ERR, curr->value), 1);
	while (curr)
	{
		if (is_unsupported_token(curr->type))
			return (ft_dprintf(STDERR_FILENO, MS_MES BADTOK_ERR, curr->value),
				1);
		if (curr->type == TOK_PIPE && is_pipe_error(curr))
			return (1);
		if (is_redir(curr->type) && is_redir_error(curr))
			return (1);
		curr = curr->next;
	}
	return (0);
}

/** @brief Master parsing function. Builds and returns AST
 */
t_ast_node	*parse(t_msdata *msdata, t_token_node *tokens)
{
	t_ast_node			*ast;
	static t_parse_vars	vars;

	vars.curr_tok = tokens;
	if (token_checks(vars.curr_tok))
	{
		msdata->last_exit = 2;
		return (NULL);
	}
	ast = parse_pipeline(msdata, &vars);
	if (!ast)
		return (NULL);
	if (vars.curr_tok)
		return (ft_dprintf(STDERR_FILENO, MS_MES SYN_ERR, tokens->value), NULL);
	return (ast);
}

/*
 *  * SHELL GRAMMAR *
 *
 * PIPE			::= '|'
 * REDIR_IN		::= '<'
 * REDIR_OUT		::= '>'
 * REDIR_APP		::= '>>'
 * HERE_DOC		::= '<<'

 * program          : pipeline
 *                  ;
 * pipeline			:               command
 *                  | pipeline PIPE command
 *                  ;
 * command			: cmd_prefix cmd_suffix
 *                  | cmd_prefix
 *                  | cmd_name cmd_suffix
 *                  | cmd_name
 *                  ;
 * cmd_name         : WORD                          // CMD
 *                  ;
 * cmd_prefix       :            io_redirect
 *                  | cmd_prefix io_redirect        // redirect list
 *                  ;
 * cmd_suffix       :            io_redirect
 *                  | cmd_suffix io_redirect
 *                  |            WORD               // ARG
 *                  | cmd_suffix WORD
 *                  ;
 * io_redirect      : io_file
 *                  | io_here
 *                  ;
 * io_file          : REDIR_IN  filename
 *                  | REDIR_OUT filename
 *                  | REDIR_APP filename
 *                  ;
 * filename         : WORD
 *                  ;
 * io_here          : HERE_DOC here_end
 *                  ;
 * here_end         : WORD
 *                  ;
*/

/* void	print_ast(t_ast_node *node, int depth)
{
	if (!node)
		return ;

	// Print indentation for current depth
	for (int i = 0; i < depth; i++)
		printf("  ");

	// Print current node type and value
	switch (node->type)
	{
		case AST_PIPELINE:
			printf("PIPELINE\n");
			break;
		case AST_COMMAND:
			printf("COMMAND\n");
			break;
		case AST_IO_REDIRECT:
			printf("IO_REDIRECT (%s)\n", node->value ? node->value : "NULL");
			break;
		case AST_WORD:
			printf("WORD (%s)\n", node->value ? node->value : "NULL");
			break;
		case AST_HEREDOC:
			printf("HEREDOC (%s)\n", node->value ? node->value : "NULL");
			break;
		case AST_FILENAME:
			printf("FILENAME (%s)\n", node->value ? node->value : "NULL");
			break;
		case AST_HEREEND:
			printf("HEREEND (%s)\n", node->value ? node->value : "NULL");
			break;
        case AST_CMD_NAME:
			printf("CMD_NAME (%s)\n", node->value ? node->value : "NULL");
			break;
        case AST_ARG:
			printf("ARG (%s)\n", node->value ? node->value : "NULL");
			break;
	}

	// Print left child if exists
	if (node->left)
	{
		for (int i = 0; i < depth + 1; i++)
			printf("  ");
		printf("|- Left:\n");
		print_ast(node->left, depth + 2);
	}

	// Print right child if exists
	if (node->right)
	{
		for (int i = 0; i < depth + 1; i++)
			printf("  ");
		printf("|- Right:\n");
		print_ast(node->right, depth + 2);
	}
}
 */
