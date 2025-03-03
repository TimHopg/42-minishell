/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 12:22:28 by thopgood          #+#    #+#             */
/*   Updated: 2025/01/27 20:19:08 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Parse a pipeline ->
 *  pipeline: command | pipeline PIPE command
 */
t_ast_node	*parse_pipeline(t_msdata *msdata, t_parse_vars *vars)
{
	t_ast_node	*command;
	t_ast_node	*pipeline;

	command = parse_command(msdata, vars);
	if (vars->curr_tok && vars->curr_tok->type == TOK_PIPE)
	{
		consume_token(&vars->curr_tok);
		pipeline = create_ast_node(msdata, AST_PIPELINE, "|");
		pipeline->left = command;
		pipeline->right = parse_pipeline(msdata, vars);
		return (pipeline);
	}
	return (command);
}

/** @brief If CMD_NAME node exists, sets the left branch to CMD_NAME, then
 *  appends the prefix node to it's rightmost node, if a prefix node exists.
 */
static void	command_name_helper(t_parse_vars *vars)
{
	vars->curr_node->left = vars->name;
	if (vars->prefix)
	{
		vars->temp_node = vars->name;
		while (vars->temp_node->right)
			vars->temp_node = vars->temp_node->right;
		vars->temp_node->right = vars->prefix;
	}
}

/** @brief When suffix is found. Uses CMD_NAME if it exists, otherwise uses
 *  prefix. If neither are found, a syntactic error has occurred (see grammar).
 *  Suffix is appended to the rightmost node of the base subtree.
 */
static void	command_suffix_helper(t_parse_vars *vars)
{
	if (vars->name)
		vars->temp_node = vars->name;
	else
		vars->temp_node = vars->prefix;
	while (vars->temp_node && vars->temp_node->right)
		vars->temp_node = vars->temp_node->right;
	if (vars->temp_node)
		vars->temp_node->right = vars->suffix;
}

/** @brief Parses a command ->
 *  cmd_prefix cmd_suffix | cmd_prefix | cmd_name cmd_suffix | cmd_name
 *  Sets prefix, name and suffix to child nodes of AST_COMMAND accordingly.
 */
t_ast_node	*parse_command(t_msdata *msdata, t_parse_vars *vars)
{
	vars->curr_node = create_ast_node(msdata, AST_COMMAND, NULL);
	vars->prefix = parse_cmd_prefix(msdata, &vars->curr_tok);
	vars->name = parse_cmd_name(msdata, &vars->curr_tok);
	vars->suffix = parse_cmd_suffix(msdata, vars);
	if (vars->name)
		command_name_helper(vars);
	else
		vars->curr_node->left = vars->prefix;
	if (vars->suffix)
		command_suffix_helper(vars);
	return (vars->curr_node);
}

/** @brief Parse a command name ->
 *  cmd_name: WORD
 */
t_ast_node	*parse_cmd_name(t_msdata *msdata, t_token_node **tokens)
{
	t_token_node	*token;

	if (*tokens && (*tokens)->type == TOK_WORD)
	{
		token = consume_token(tokens);
		return (create_ast_node(msdata, AST_CMD_NAME, token->value));
	}
	return (NULL);
}
