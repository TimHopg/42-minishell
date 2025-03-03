/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npizzi <npizzi@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:37:38 by thopgood          #+#    #+#             */
/*   Updated: 2025/01/08 09:29:15 by npizzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_dprintchar(int fd, int c)
{
	return (write(fd, &c, 1));
}

int	ft_dprintstr(int fd, char *str)
{
	int	count;

	count = 0;
	if (str == NULL)
	{
		if (write(fd, NULL_MSG, ft_strlen(NULL_MSG)) == -1)
			return (-1);
		return (ft_strlen(NULL_MSG));
	}
	while (*str)
	{
		if (ft_dprintchar(fd, (int)*str) == -1)
			return (-1);
		++count;
		++str;
	}
	return (count);
}
