/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_jump_space.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 17:34:42 by g-alves-          #+#    #+#             */
/*   Updated: 2026/03/27 16:43:07 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_jump_space(char **string)
{
	int	index;

	index = 0;

	if (!string || !*string || !**string)
		return (1);
	while ((*string)[index] && ((*string)[index] == ' '
			|| ((*string)[index] >= 9 && (*string)[index] <= 13)))
		index++;
	*string += index;
	return (0);
}
