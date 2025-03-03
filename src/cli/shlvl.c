/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 00:13:34 by npizzi            #+#    #+#             */
/*   Updated: 2025/02/10 13:53:07 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_only_spaces(char *input)
{
	int	i;

	i = 0;
	if (!input)
		return (true);
	while (input[i])
	{
		if (!isspace(input[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	is_only_digit(char *shlvl)
{
	int	i;

	if (!shlvl || !shlvl[0])
		return (false);
	i = 0;
	while (shlvl[i])
	{
		if (!ft_isdigit(shlvl[i]) && !ft_isspace(shlvl[i]) && shlvl[i] != '+'
			&& shlvl[i] != '-')
			return (false);
		i++;
	}
	return (true);
}

/** @brief Returns new shell level. If level is greater than 1000 returns 1
 * 	and prints error. Returns 0 if level is negative.
 */
int	new_shlvl(char *shlvl)
{
	int	new_lvl;

	if (!is_only_digit(shlvl))
		return (1);
	new_lvl = ft_atoi(shlvl) + 1;
	if (new_lvl >= 1000)
	{
		ft_dprintf(STDERR_FILENO, MS_MES WARN_MES SHLVL_ERR, new_lvl);
		new_lvl = 1;
	}
	if (new_lvl < 0)
		new_lvl = 0;
	return (new_lvl);
}

/** @brief Updates current shell level. If new level would exceed 1000 prints
 *  error message and sets to 1 (exits with code 2).
 *  If SHLVL var is not set, empty or contains non digits sets SHLVL to 1.
 */
void	update_shell_level(t_msdata *msdata)
{
	char	*shlvl;
	char	*shlvl_var;

	shlvl = get_env_value("SHLVL", msdata->env);
	shlvl = ft_itoa(new_shlvl(shlvl));
	if (!shlvl)
		fatal_error(msdata, MALLOC_ERR, MALLOC_ERRCD);
	shlvl_var = ft_strjoin("SHLVL=", shlvl);
	safe_free(&shlvl);
	if (!shlvl_var)
		fatal_error(msdata, MALLOC_ERR, MALLOC_ERRCD);
	msdata->env = add_var_to_env(shlvl_var, msdata->env);
	safe_free(&shlvl_var);
	if (!msdata->env)
		fatal_error(msdata, MALLOC_ERR, MALLOC_ERRCD);
}
