/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenise_meta2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 13:25:04 by thopgood          #+#    #+#             */
/*   Updated: 2025/01/27 20:23:11 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tokenise_redir(char **input, t_msdata *msdata)
{
	if (**input == '<')
	{
		if (*(*input + 1) == '<')
		{
			lst_append(&msdata->token_list, new_token_node(ft_substr(*input, 0,
						2), TOK_HEREDOC, msdata));
			(*input)++;
		}
		else
			lst_append(&msdata->token_list, new_token_node(ft_substr(*input, 0,
						1), TOK_REDIR_IN, msdata));
		return ;
	}
	if (**input == '>')
	{
		if (*(*input + 1) == '>')
		{
			lst_append(&msdata->token_list, new_token_node(ft_substr(*input, 0,
						2), TOK_REDIR_APP, msdata));
			(*input)++;
		}
		else
			lst_append(&msdata->token_list, new_token_node(ft_substr(*input, 0,
						1), TOK_REDIR_OUT, msdata));
	}
}

void	tokenise_quotes(char **input, t_msdata *msdata)
{
	int		i;
	char	quote;
	char	*token;

	i = 1;
	quote = **input;
	while ((*input)[i] && (*input)[i] != quote)
		i++;
	token = ft_substr(*input, 0, i + 1);
	if (!token)
		fatal_error(msdata, MALLOC_ERR, MALLOC_ERRCD);
	if (has_unclosed_quotes(token))
		return (free_list(&msdata->token_list));
	if (quote == SINGLE_QUOTE)
		lst_append(&msdata->token_list, new_token_node(token, TOK_SNGL_QT,
				msdata));
	else if (quote == DOUBLE_QUOTE)
		lst_append(&msdata->token_list, new_token_node(token, TOK_DBL_QT,
				msdata));
	*input += i;
}

void	tokenise_paren(char **input, t_msdata *msdata)
{
	if (**input == '(')
		lst_append(&msdata->token_list, new_token_node(ft_substr(*input, 0, 1),
				TOK_OPN_PAREN, msdata));
	else if (**input == ')')
		lst_append(&msdata->token_list, new_token_node(ft_substr(*input, 0, 1),
				TOK_CLS_PAREN, msdata));
}
