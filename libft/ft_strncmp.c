/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 20:30:01 by g-alves-          #+#    #+#             */
/*   Updated: 2025/09/01 14:11:50 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	index;

	index = 0;
	if (n == 0)
		return (0);
	while ((*s1 && *s2) && n > index)
	{
		if (!((unsigned char)*s1 == (unsigned char)*s2))
			return ((unsigned char)*s1 - (unsigned char)*s2);
		index++;
		s1++;
		s2++;
	}
	if (index == n)
		return (0);
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
