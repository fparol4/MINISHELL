/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 12:48:11 by g-alves-          #+#    #+#             */
/*   Updated: 2025/09/01 15:24:03 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	size_t	index_string;
	int		sign;
	int		number;

	index_string = 0;
	while ((nptr[index_string] == ' ')
		|| ((nptr[index_string] >= 9)
			&& (nptr[index_string] <= 13)))
		index_string++;
	sign = 1;
	if ((nptr[index_string] == '-') || (nptr[index_string] == '+'))
	{
		if (nptr[index_string] == '-')
			sign = -1;
		index_string++;
	}
	number = 0;
	while ((nptr[index_string] >= '0') && (nptr[index_string] <= '9'))
	{
		number = (number * 10) + (nptr[index_string] - '0');
		index_string++;
	}
	return (sign * number);
}
