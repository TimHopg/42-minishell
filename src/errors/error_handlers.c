/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 11:33:07 by npizzi            #+#    #+#             */
/*   Updated: 2025/02/10 14:54:05 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_err(char *str)
{
	write(STDERR_FILENO, str, strlen(str));
	safe_free(&str);
}

void	*safe_calloc(t_msdata *msdata, size_t n)
{
	void	*p;

	p = malloc(n);
	if (p == NULL)
	{
		ft_dprintf(STDERR_FILENO, "Failed to allocate %u bytes.\n", n);
		fatal_error(msdata, MALLOC_ERR, MALLOC_ERRCD);
	}
	ft_bzero(p, n);
	return (p);
}

void	fatal_error(t_msdata *msdata, char *err_msg, int errcode)
{
	if (errno)
	{
		errcode = errno;
		perror(err_msg);
	}
	else
	{
		ft_dprintf(STDERR_FILENO, MS_MES "%s", err_msg);
	}
	free_msdata(msdata);
	exit(errcode);
}
