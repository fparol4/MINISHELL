/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 16:17:48 by g-alves-          #+#    #+#             */
/*   Updated: 2025/09/01 15:16:26 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			index;
	unsigned char	*pointer_converted;

	index = 0;
	pointer_converted = (unsigned char *)s;
	while (index < n)
	{
		pointer_converted[index] = c;
		index++;
	}
	return (s);
}
