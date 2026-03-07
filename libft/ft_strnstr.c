/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:21:24 by g-alves-          #+#    #+#             */
/*   Updated: 2026/03/06 23:23:11 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	index_len;
	size_t	index_little;
	size_t	index_big;

	index_len = len;
	index_little = 0;
	index_big = 0;
	if (*little == '\0')
		return ((char *)big);
	while (index_len > index_big && big[index_big] != '\0')
	{
		if (big[index_big] == little[index_little])
		{
			index_little = 0;
			while (little[index_little] != '\0'
				&& index_len > (index_big + index_little)
				&& big[index_big + index_little] == little[index_little])
				index_little++;
			if (little[index_little] == '\0')
				return ((char *)&big[index_big]);
		}
		index_big++;
		index_little = 0;
	}
	return (NULL);
}
