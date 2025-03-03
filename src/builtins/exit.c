/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 19:06:41 by npizzi            #+#    #+#             */
/*   Updated: 2025/01/27 19:50:11 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Checks string is [optional whitespace, single +/-, at least one
 *  digit followed by further optional white space].
 */
bool	is_valid_pattern(char *str)
{
	while (ft_isspace(*str))
		str++;
	if (*str == '+' || *str == '-')
		str++;
	if (!ft_isdigit(*str))
		return (false);
	while (ft_isdigit(*str))
		str++;
	while (ft_isspace(*str))
		str++;
	if (*str != '\0')
		return (false);
	return (true);
}

/** @brief Checks if arg is empty string, doesn't adhere to pattern rules
 *  is out of range of LLONG and if so, prints error message.
 */
bool	is_arg_valid(char *exit_arg)
{
	bool	is_valid;
	bool	out_of_range;

	is_valid = true;
	out_of_range = false;
	if (exit_arg[0] == 0 || !is_valid_pattern(exit_arg))
		is_valid = false;
	ft_atoill(exit_arg, &out_of_range);
	if (out_of_range)
		is_valid = false;
	if (!is_valid)
		ft_dprintf(STDERR_FILENO, MS_MES EXIT_NUM_ERR, exit_arg);
	return (is_valid);
}

/** @brief Safe frees, clears history and exits with parameter exit_status.
 */
static void	exit_exit(t_msdata *msdata, char **args, int exit_status)
{
	free_msdata(msdata);
	safe_free_array(&args);
	rl_clear_history();
	exit(exit_status);
}

/** @brief Minishell exit built-in.
 */
void	ms_exit(t_msdata *msdata, char **args)
{
	int		exit_status;
	bool	is_numeric;

	if (args && args[1])
	{
		if (ft_strncmp(args[1], "--", 3) == 0)
			exit_exit(msdata, args, 0);
		is_numeric = is_arg_valid(args[1]);
		if (is_numeric && args[2])
		{
			ft_dprintf(STDERR_FILENO, MS_MES EXITARGS_ERR);
			msdata->last_exit = 1;
			return ;
		}
		if (is_numeric)
			exit_status = (ft_atoill(args[1], NULL) % 256);
		else
			exit_status = 2;
	}
	else
		exit_status = msdata->last_exit;
	exit_exit(msdata, args, exit_status);
}
