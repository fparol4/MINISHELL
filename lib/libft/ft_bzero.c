/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 11:38:39 by g-alves-          #+#    #+#             */
/*   Updated: 2025/07/29 12:19:04 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned int	index;
	unsigned char	*casting_void;

	index = 0;
	casting_void = (unsigned char *)s;
	while (index < n)
	{
		casting_void[index] = 0;
		index++;
	}
}
