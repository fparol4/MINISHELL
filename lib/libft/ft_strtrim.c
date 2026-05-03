/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 16:15:32 by g-alves-          #+#    #+#             */
/*   Updated: 2025/09/01 23:00:53 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	long int	start_string;
	long int	end_string;
	char		*full_trim;

	start_string = -1;
	while (s1[++start_string])
		if (!ft_strchr(set, s1[start_string]))
			break ;
	end_string = ft_strlen(s1);
	while (s1[--end_string])
		if (!ft_strchr(set, s1[end_string]))
			break ;
	full_trim = ft_substr(s1, start_string, (end_string - start_string +1));
	return (full_trim);
}
