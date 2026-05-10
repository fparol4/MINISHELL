/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_char_table.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 12:55:43 by g-alves-          #+#    #+#             */
/*   Updated: 2026/05/04 18:25:10 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra.h"

void	init_char_table(t_char_table *table)
{
	int	i;

	i = 0;
	while (i < 256)
		table->props[i++] = 0;
	i = 0;
	while (i < 256)
	{
		if (ft_isalpha(i))
			table->props[i] |= P_ALPHA;
		else if (ft_isdigit(i))
			table->props[i] |= P_DIGIT;
		else if ((i >= 9 && i <= 13) || (i == 32))
			table->props[i] |= P_SPACE;
		else if (i == 39)
			table->props[i] |= P_SQUOTE;
		else if (i == 34)
			table->props[i] |= P_DQUOTE;
		else if (i == 92)
			table->props[i] |= P_ESCAPE;
		else
			table->props[i] |= P_SYMBOL;
		i++;
	}
}
