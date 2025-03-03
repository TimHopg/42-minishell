/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 12:44:31 by thopgood          #+#    #+#             */
/*   Updated: 2025/02/05 11:51:28 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*advance_pathname(char *pathname, char *wildcard)
{
	int	i;
	int	x;

	i = 0;
	x = 0;
	while (wildcard[i] && wildcard[i] != '*')
		i++;
	if (wildcard[i] == 0)
		i++;
	while (pathname[x])
	{
		if (ft_strncmp(pathname + x, wildcard, i) == 0)
		{
			return (pathname + x + i);
		}
		x++;
	}
	return (NULL);
}

bool	string_matches(char *pathname, char *wildcard)
{
	while (*wildcard && (*wildcard == '*' || *pathname == *wildcard))
	{
		if (*wildcard == '*')
		{
			while (*wildcard == '*')
				wildcard++;
			if (!*wildcard)
				return (true);
			pathname = advance_pathname(pathname, wildcard);
			if (!pathname)
				return (false);
			while (*wildcard && *wildcard != '*')
				wildcard++;
		}
		else
		{
			wildcard++;
			pathname++;
		}
	}
	if (!*wildcard && !*pathname)
		return (true);
	return (false);
}

void	change_token_node(char *pathname, char *wildcard, t_token_node **node,
		t_msdata *msdata)
{
	char	*pathname_copy;

	if (pathname[0] == '.' && wildcard[0] != '.')
		return ;
	if (string_matches(pathname, wildcard))
	{
		pathname_copy = ft_strdup(pathname);
		if (!pathname)
			fatal_error(msdata, MALLOC_ERR, MALLOC_ERRCD);
		if ((*node)->type == TOK_WILDCARD)
		{
			safe_free(&(*node)->value);
			(*node)->value = pathname_copy;
			(*node)->type = TOK_WORD;
		}
		else
			add_token_in_place(pathname_copy, TOK_WORD, node, msdata);
	}
}

void	expand_wildcard(char *wildcard, t_token_node **node, t_msdata *msdata)
{
	DIR				*cwd;
	struct dirent	*entry;

	cwd = opendir("./");
	if (!cwd)
	{
		ft_dprintf(STDERR_FILENO, MS_MES "error: %s", strerror(errno));
		return ;
	}
	errno = 0;
	entry = readdir(cwd);
	while (entry)
	{
		change_token_node(entry->d_name, wildcard, node, msdata);
		entry = readdir(cwd);
	}
	if (!entry && errno)
	{
		ft_dprintf(STDERR_FILENO, MS_MES "error: %s", strerror(errno));
	}
	closedir(cwd);
}

void	expand_wildcards(t_token_node **tokens_list, t_msdata *msdata)
{
	char			*wildcard_copy;
	t_token_node	*iterator;

	iterator = *tokens_list;
	while (iterator)
	{
		if (iterator->type == TOK_WILDCARD)
		{
			wildcard_copy = ft_strdup(iterator->value);
			if (!wildcard_copy)
				fatal_error(msdata, MALLOC_ERR, MALLOC_ERRCD);
			expand_wildcard(wildcard_copy, &iterator, msdata);
			if (iterator->type == TOK_WILDCARD)
				iterator->type = TOK_WORD;
			safe_free(&wildcard_copy);
		}
		iterator = iterator->next;
	}
}
