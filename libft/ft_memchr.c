/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 21:21:09 by g-alves-          #+#    #+#             */
/*   Updated: 2025/09/01 15:04:02 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t					index;
	const unsigned char		*string;

	string = s;
	index = 0;
	while (n > index)
	{
		if (string[index] == (unsigned char)c)
			return ((void *)(string + index));
		index++;
	}
	return (NULL);
}
