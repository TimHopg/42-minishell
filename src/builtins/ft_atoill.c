/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoill.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:26:58 by thopgood          #+#    #+#             */
/*   Updated: 2025/01/27 16:30:22 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/** @brief Helper ft_atoill function to process digits and check next iteration
 * 	won't be out of range.
 */
static long long	atoi_process_digits(const char *nptr, int sign,
		bool *out_of_range)
{
	long long	output;

	output = 0;
	while (ft_isdigit(*nptr))
	{
		if (sign == -1 && output == (-(LLONG_MIN + 1) / 10) && (*nptr
				- '0') == (-(LLONG_MIN % 10)))
		{
			output = output * 10 + (*nptr - '0');
			nptr++;
			break ;
		}
		if (output > (LLONG_MAX - (*nptr - '0')) / 10)
		{
			if (out_of_range)
				*out_of_range = true;
			if (sign == 1)
				return (LLONG_MAX);
			else
				return (LLONG_MIN);
		}
		output = (output * 10) + (*nptr - '0');
		nptr++;
	}
	return (output);
}

/** @brief Modified atoi function to handle long long and set out_of_range to
 *  true if resulting value is < LLONG_MIN or > LLONG_MAX
 */
long long	ft_atoill(const char *nptr, bool *out_of_range)
{
	long long	output;
	int			sign;

	sign = 1;
	if (out_of_range)
		*out_of_range = false;
	while (ft_isspace(*nptr))
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	output = atoi_process_digits(nptr, sign, out_of_range);
	return (output * sign);
}
