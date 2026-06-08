/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 20:42:18 by g-alves-          #+#    #+#             */
/*   Updated: 2026/06/04 20:42:18 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_lexer.h"

int	state_machine(t_manager *manager, t_scanner *input, t_rules *rules)
{
	unsigned int	props;

	if (!input)
		return (1);
	while (!scanner_is_end(input))
	{
		props = rules->table.props[scanner_current(input)];
		if (props & rules->start_operator)
			extract_operator(manager, input, rules);
		else if (props & rules->start_word)
		{
			if (!extract_word(manager, input, rules))
				return (0);
		}
		else
			scanner_advance(input);
	}
	return (1);
}
