/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_jump_space.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 17:34:42 by g-alves-          #+#    #+#             */
/*   Updated: 2026/03/07 17:43:13 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_jump_space(char *string)
{
	int	index;

	index = 0;

	if (!string)
		return ;
	while (string[index] && (string[index] == ' ' || (string[index] >= 9
				&& string[index] <= 13)))
		index++;
	string + index;
}
