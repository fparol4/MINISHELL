/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:13:48 by g-alves-          #+#    #+#             */
/*   Updated: 2025/09/02 14:23:53 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned int	index_cpy;
	unsigned char	*origin;
	unsigned char	*destiny;

	origin = (unsigned char *)src;
	destiny = (unsigned char *)dest;
	if (!dest && !src)
		return (NULL);
	index_cpy = 0;
	while (index_cpy < n)
	{
		destiny[index_cpy] = origin[index_cpy];
		index_cpy++;
	}
	return (dest);
}
