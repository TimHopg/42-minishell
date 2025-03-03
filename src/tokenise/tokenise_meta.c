/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenise_meta.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 13:22:31 by thopgood          #+#    #+#             */
/*   Updated: 2025/02/05 11:49:11 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tokenise_wildcard(char **input, t_msdata *msdata)
{
	int	i;

	i = 0;
	while (*input[i] && ft_strchr("*", *input[i]))
		i++;
	lst_append(&(msdata->token_list), new_token_node(ft_substr(*input, 0, i),
			TOK_WILDCARD, msdata));
	*input += i - 1;
}

void	tokenise_pipe_or(char **input, t_msdata *msdata)
{
	if (**input == '|')
	{
		if (*(*input + 1) == '|')
		{
			lst_append(&msdata->token_list, new_token_node(ft_substr(*input, 0,
						2), TOK_OR, msdata));
			(*input)++;
			return ;
		}
		else
			lst_append(&msdata->token_list, new_token_node(ft_substr(*input, 0,
						1), TOK_PIPE, msdata));
	}
}

void	tokenise_and(char **input, t_msdata *msdata)
{
	if (**input == '&')
	{
		if (*(*input + 1) == '&')
		{
			lst_append(&msdata->token_list, new_token_node(ft_substr(*input, 0,
						2), TOK_AND, msdata));
			(*input)++;
			return ;
		}
		else
			lst_append(&msdata->token_list, new_token_node(ft_substr(*input, 0,
						1), TOK_UNKNOWN, msdata));
	}
}

void	tokenise_env_var(char **input, t_msdata *msdata)
{
	int	i;

	i = 1;
	if ((*input)[i] == '?')
		i++;
	else
	{
		while (is_env_var_valid_char((*input)[i]))
			i++;
	}
	if (i == 1)
	{
		lst_append(&(msdata->token_list), new_token_node(ft_substr(*input, 0,
					i), TOK_WORD, msdata));
		return ;
	}
	lst_append(&msdata->token_list, new_token_node(ft_substr(*input, 0, i),
			TOK_ENV_VAR, msdata));
	*input += +i - 1;
}

void	tokenise_metachar(char **input, t_msdata *msdata)
{
	if (**input == '*')
		tokenise_wildcard(input, msdata);
	if (**input == '$')
		tokenise_env_var(input, msdata);
	if (**input == '&')
		tokenise_and(input, msdata);
	if (**input == '|')
		tokenise_pipe_or(input, msdata);
	if (**input == '<' || **input == '>')
		tokenise_redir(input, msdata);
	if (**input == '"' || **input == '\'')
		tokenise_quotes(input, msdata);
	if (**input == '(' || **input == ')')
		tokenise_paren(input, msdata);
}
