/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenise.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 10:45:55 by thopgood          #+#    #+#             */
/*   Updated: 2025/02/10 15:18:24 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Checks if current token should be joined with following token
 */
void	check_for_combine(char *input, t_token_node *token)
{
	t_token_type	type;

	type = token->type;
	if (!input || input[0] == 0 || input[1] == 0)
		return ;
	if (type == TOK_WORD && token->value[0] == '$' && token->value[1] == 0)
	{
		if (input[1] == '\'' || input[1] == '"')
		{
			token->type = TOK_ENV_VAR;
			token->combine_next = true;
		}
	}
	if (type == TOK_DBL_QT || type == TOK_SNGL_QT || type == TOK_WORD
		|| type == TOK_ENV_VAR || type == TOK_WILDCARD)
	{
		if (!ft_isspace(input[1]) && !ft_strchr("<>|&()", input[1]))
			token->combine_next = true;
	}
}

void	tokenise_word(char **input, t_msdata *msdata)
{
	int	i;

	i = 0;
	while ((*input)[i] && !ft_isspace((*input)[i]) && !is_metachar((*input)[i]))
		i++;
	if ((*input)[i] == '$' && (ft_isspace((*input)[i + 1])
			|| is_metachar((*input)[i + 1]) || (*input)[i + 1] == '\0'))
		i++;
	lst_append(&msdata->token_list, new_token_node(ft_substr(*input, 0, i),
			TOK_WORD, msdata));
	*input += i - 1;
}

/** @brief Traverses input string and tokenises metacharacters and words
 */
void	token_split(char *input, t_msdata *msdata)
{
	while (*input)
	{
		if (is_metachar(*input))
		{
			tokenise_metachar(&input, msdata);
			if (!msdata->token_list)
				return ;
			check_for_combine(input, lstlast(msdata->token_list));
		}
		else if (!ft_isspace(*input))
		{
			tokenise_word(&input, msdata);
			check_for_combine(input, lstlast(msdata->token_list));
		}
		input++;
	}
}

/** @brief Master tokenise function
 */
t_token_node	*tokenise(t_msdata *msdata)
{
	char	*trimmed_input;

	trimmed_input = ft_strtrim(msdata->input, " ");
	if (!trimmed_input)
		fatal_error(msdata, MALLOC_ERR, MALLOC_ERRCD);
	token_split(trimmed_input, msdata);
	safe_free(&trimmed_input);
	if (!msdata->token_list)
		return (NULL);
	msdata->has_input = true;
	if (count_heredocs(msdata->token_list) > MAX_HEREDOCS_NUM)
		fatal_error(msdata, HDOCMAX_ERR, HDOCMAX_ERRCD);
	adjust_heredoc_delimiter(msdata->token_list);
	expand_tokens(&msdata->token_list, msdata);
	combine_tokens(msdata, msdata->token_list);
	expand_wildcards(&msdata->token_list, msdata);
	return (msdata->token_list);
}
