/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 21:49:04 by g-alves-          #+#    #+#             */
/*   Updated: 2025/09/01 14:56:52 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t					index;
	const unsigned char		*first_string;
	const unsigned char		*second_string;

	first_string = s1;
	second_string = s2;
	index = 0;
	if (n == index)
		return (0);
	while (n > index)
	{
		if (!(first_string[index] == second_string[index]))
			return (first_string[index] - second_string[index]);
		index++;
	}
	return (0);
}
