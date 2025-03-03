/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:59:12 by thopgood          #+#    #+#             */
/*   Updated: 2025/02/05 11:59:14 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token_in_place(char *value, t_token_type tok_type,
		t_token_node **node, t_msdata *msdata)
{
	t_token_node	*new_token;

	new_token = new_token_node(value, tok_type, msdata);
	new_token->next = (*node)->next;
	(*node)->next = new_token;
	(*node) = new_token;
}

bool	is_space_present(char *expanded_var)
{
	int	i;

	i = 0;
	while (expanded_var[i])
	{
		if (expanded_var[i] == ' ')
			return (true);
		i++;
	}
	return (false);
}

void	make_words_tokens(t_token_node **token, char **splitted_word,
		t_msdata *msdata)
{
	int	i;

	i = 1;
	if (!splitted_word[0])
	{
		((*token)->value) = ft_strdup("");
		if (!(*token)->value)
			fatal_error(msdata, MALLOC_ERR, MALLOC_ERRCD);
		return ;
	}
	(*token)->value = splitted_word[0];
	(*token)->combine_next = false;
	while (splitted_word[i])
	{
		add_token_in_place(splitted_word[i], TOK_WORD, token, msdata);
		if (ft_strchr((*token)->value, '*'))
			(*token)->type = TOK_WILDCARD;
		i++;
	}
}

void	split_word(t_token_node **token, t_msdata *msdata)
{
	char	**splitted_word;

	if (!(*token)->value || !(*token)->value[0])
		return ;
	if (ft_strchr((*token)->value, '*'))
		(*token)->type = TOK_WILDCARD;
	if (!is_space_present((*token)->value))
		return ;
	splitted_word = ft_split((*token)->value, ' ');
	safe_free(&((*token)->value));
	if (!splitted_word)
		fatal_error(msdata, MALLOC_ERR, MALLOC_ERRCD);
	make_words_tokens(token, splitted_word, msdata);
	free(splitted_word);
}
