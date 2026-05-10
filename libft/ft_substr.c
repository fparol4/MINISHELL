/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 19:54:19 by g-alves-          #+#    #+#             */
/*   Updated: 2025/09/02 14:32:34 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	index_current;
	size_t	len_string;
	char	*sub_string;

	if (!s)
		return (NULL);
	len_string = ft_strlen(s);
	index_current = 0;
	if (start >= len_string)
		len = index_current;
	if (len > (len_string - start))
		len = len_string - start;
	sub_string = malloc(len + 1);
	if (!sub_string)
		return (NULL);
	while ((s[index_current] != '\0') && (index_current < len))
	{
		sub_string[index_current] = s[index_current + start];
		index_current++;
	}
	sub_string[index_current] = '\0';
	return (sub_string);
}
