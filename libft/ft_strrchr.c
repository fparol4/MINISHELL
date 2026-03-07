/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 14:48:19 by g-alves-          #+#    #+#             */
/*   Updated: 2025/09/01 15:06:49 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char	*ft_strrchr(const char *s, int c)
{
	size_t	index;

	index = ft_strlen(s);
	while (index > 0)
	{
		if (s[index] == (unsigned char)c)
			return ((char *)(s + index));
		index--;
	}
	if (s[index] == (unsigned char)c)
		return ((char *)(s + index));
	return (NULL);
}
