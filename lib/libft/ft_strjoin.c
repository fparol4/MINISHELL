/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 12:17:06 by g-alves-          #+#    #+#             */
/*   Updated: 2025/09/01 12:52:48 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*full_string;
	size_t	index_str;

	if (!s1 || !s2)
		return (NULL);
	full_string = malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * (sizeof(char)));
	if (!full_string)
		return (NULL);
	index_str = 0;
	while (*s1)
	{
		full_string[index_str] = *s1;
		index_str++;
		s1++;
	}
	while (*s2)
	{
		full_string[index_str] = *s2;
		index_str++;
		s2++;
	}
	full_string[index_str] = '\0';
	return (full_string);
}
