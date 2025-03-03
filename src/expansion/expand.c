/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 16:58:00 by npizzi            #+#    #+#             */
/*   Updated: 2025/02/10 14:55:00 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_simple_var(char *var, t_msdata *msdata)
{
	char	*var_value;
	char	*expanded_var;

	if (!var)
		return (NULL);
	if (var[0] == '?')
		return (ft_itoa(msdata->last_exit));
	var_value = get_env_value(var, msdata->env);
	if (var_value)
		expanded_var = ft_strdup(var_value);
	else
		expanded_var = ft_strdup("");
	return (expanded_var);
}

char	*replace_string(char *string_to_replace, int *index, int var_len,
		t_msdata *msdata)
{
	char	*token_substring;
	char	*expanded_var;
	char	*joined_string;
	char	*var;

	token_substring = ft_substr(string_to_replace, 0, *index - 1);
	if (!token_substring)
		return (NULL);
	var = ft_substr(string_to_replace, *index, var_len);
	if (!var)
		return (free(token_substring), NULL);
	expanded_var = expand_simple_var(var, msdata);
	joined_string = ft_strjoin(token_substring, expanded_var);
	safe_free(&var);
	safe_free(&token_substring);
	safe_free(&expanded_var);
	if (!joined_string)
		return (NULL);
	expanded_var = ft_strjoin(joined_string, string_to_replace + (*index)
			+ var_len);
	*index = ft_strlen(joined_string);
	safe_free(&joined_string);
	safe_free(&string_to_replace);
	return (expanded_var);
}

char	*expand_quoted_var(char *token, t_msdata *msdata)
{
	int	i;
	int	x;

	i = 0;
	while (token && token[i])
	{
		x = 0;
		if (token[i] == '$')
		{
			i++;
			if (token[i] == '?')
				token = replace_string(token, &i, 1, msdata);
			else if (is_env_var_valid_char(token[i]) && token[i])
			{
				while (is_env_var_valid_char(token[i + x]) && token[i + x])
					x++;
				token = replace_string(token, &i, x, msdata);
			}
		}
		else
			i++;
	}
	return (token);
}

void	expand_tokens(t_token_node **tokens_list, t_msdata *msdata)
{
	t_token_node	*temp;
	t_token_type	type;
	char			*var_value;

	temp = *tokens_list;
	while (temp)
	{
		type = temp->type;
		if (type == TOK_DBL_QT)
			temp->value = expand_quoted_var(temp->value, msdata);
		else if (type == TOK_ENV_VAR)
		{
			var_value = expand_simple_var(temp->value + 1, msdata);
			safe_free(&(temp->value));
			temp->value = var_value;
			split_word(&temp, msdata);
		}
		if (type == TOK_SNGL_QT || type == TOK_DBL_QT)
			remove_quotes_in_place(&(temp->value));
		if (!(temp->value))
			fatal_error(msdata, MALLOC_ERR, MALLOC_ERRCD);
		temp = temp->next;
	}
	update_tokens_list(tokens_list);
}
