/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pref_suf.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 12:21:54 by thopgood          #+#    #+#             */
/*   Updated: 2025/01/27 20:19:14 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Parse command prefix (redirects before command)
 * cmd_prefix: io_redirect | cmd_prefix io_redirect
 * If no prefixes exist, current redirect is set to prefix otherwise traverses
 * prefix branch and sets the redirect to the end.
 */
t_ast_node	*parse_cmd_prefix(t_msdata *msdata, t_token_node **tokens)
{
	t_ast_node	*prefix;
	t_ast_node	*redirect;
	t_ast_node	*current;

	prefix = NULL;
	while (*tokens && is_redir((*tokens)->type))
	{
		redirect = parse_io_redirect(msdata, tokens);
		if (!prefix)
			prefix = redirect;
		else
		{
			current = prefix;
			while (current->right)
				current = current->right;
			current->right = redirect;
		}
	}
	return (prefix);
}

/*
 * Parse command suffix (arguments and redirects after command)
 * cmd_suffix: io_redirect | cmd_suffix io_redirect | WORD | cmd_suffix WORD
 * Creates suffix branch.
 */
t_ast_node	*parse_cmd_suffix(t_msdata *msdata, t_parse_vars *vars)
{
	vars->suffix = NULL;
	while (vars->curr_tok && (is_redir(vars->curr_tok->type)
			|| vars->curr_tok->type == TOK_WORD))
	{
		vars->temp_suf = NULL;
		if (is_redir(vars->curr_tok->type))
			vars->temp_suf = parse_io_redirect(msdata, &vars->curr_tok);
		else if (vars->curr_tok->type == TOK_WORD)
		{
			vars->word_tok = consume_token(&vars->curr_tok);
			vars->temp_suf = create_ast_node(msdata, AST_ARG,
					vars->word_tok->value);
		}
		if (!vars->suffix)
			vars->suffix = vars->temp_suf;
		else
		{
			vars->temp_node = vars->suffix;
			while (vars->temp_node->right)
				vars->temp_node = vars->temp_node->right;
			vars->temp_node->right = vars->temp_suf;
		}
	}
	return (vars->suffix);
}
