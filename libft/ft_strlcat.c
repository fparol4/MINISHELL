/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 18:34:57 by g-alves-          #+#    #+#             */
/*   Updated: 2025/09/02 15:06:05 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	index;
	size_t	size_dst;
	size_t	size_src;

	index = 0;
	size_dst = ft_strlen(dst);
	size_src = ft_strlen(src);
	if ((!dst || !src) && size == 0)
		return (0);
	if (size <= size_dst)
		return (size + size_src);
	while (size_dst + index < (size -1) && src[index] != '\0')
	{
		dst[index + size_dst] = src[index];
		index++;
	}
	dst[index + size_dst] = '\0';
	return (size_dst + size_src);
}
