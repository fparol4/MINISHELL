/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 12:25:47 by g-alves-          #+#    #+#             */
/*   Updated: 2025/09/02 14:59:04 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	index;

	index = 0;
	while ((unsigned char)s[index])
	{
		if ((unsigned char)s[index] == (unsigned char)c)
			return ((char *)&s[index]);
		index++;
	}
	if ((unsigned char)c == '\0')
		return ((char *)&s[index]);
	return (NULL);
}
