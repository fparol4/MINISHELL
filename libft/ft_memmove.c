/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:49:26 by g-alves-          #+#    #+#             */
/*   Updated: 2025/09/02 14:24:53 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned int	index;
	unsigned char	*destiny;
	unsigned char	*origin;

	index = 0;
	destiny = (unsigned char *)dest;
	origin = (unsigned char *)src;
	if (!dest && !src)
		return (NULL);
	if (destiny > origin)
	{
		while (n--)
			destiny[n] = origin[n];
	}
	else
	{
		while (index < n)
		{
			destiny[index] = origin[index];
			index++;
		}
	}
	return (dest);
}
