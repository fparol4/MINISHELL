/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 18:09:16 by g-alves-          #+#    #+#             */
/*   Updated: 2026/03/06 14:46:23 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*string_duplicate;

	len = ft_strlen(s) +1;
	string_duplicate = malloc(len);
	if (!string_duplicate)
		return (NULL);
	ft_memcpy(string_duplicate, s, len);
	return (string_duplicate);
}
