/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array_size.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 09:12:19 by npizzi            #+#    #+#             */
/*   Updated: 2025/01/21 12:58:11 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_array_size(char **array)
{
	int	array_size;

	array_size = 0;
	if (!array)
		return (0);
	while (array[array_size])
		array_size++;
	return (array_size);
}
