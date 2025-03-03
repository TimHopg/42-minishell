/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenise_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 13:28:31 by thopgood          #+#    #+#             */
/*   Updated: 2025/02/10 15:00:34 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Traverses input string and tokenises metacharacters and words
 *  @return
 *	- -1: error (string does not begin with quote mark).
 *	-  1: has unclosed quotes.
 *	-  0: has closed quotes.
 */
bool	has_unclosed_quotes(char *token)
{
	char	c;
	int		i;
	int		len;

	i = 0;
	c = token[i];
	if (c != '"' && c != '\'')
		return (false);
	len = ft_strlen(token);
	if (len == 1 || c != token[len - 1])
	{
		safe_free(&token);
		ft_dprintf(STDERR_FILENO, MS_MES UNCLOSED_QTS_ERR);
		return (true);
	}
	return (false);
}

bool	is_metachar(char c)
{
	return (ft_strchr_no_null("$&\"\'()<>|*", c));
}

bool	is_env_var_valid_char(char c)
{
	return (ft_isalnum(c) || ft_strchr_no_null("_?", c));
}

void	combine_tokens(t_msdata *msdata, t_token_node *list)
{
	t_token_node	*token;
	t_token_node	*next_token;
	char			*joined_value;

	token = list;
	while (token && token->next)
	{
		while (token->combine_next)
		{
			joined_value = ft_strjoin(token->value, token->next->value);
			if (!joined_value)
				fatal_error(msdata, MALLOC_ERR, MALLOC_ERRCD);
			token->combine_next = token->next->combine_next;
			if (token->next->type == TOK_WILDCARD)
				token->type = TOK_WILDCARD;
			next_token = token->next->next;
			safe_free(&token->next->value);
			free(token->next);
			safe_free(&token->value);
			token->next = next_token;
			token->value = joined_value;
		}
		token = token->next;
	}
}
