/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rules.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 20:42:03 by g-alves-          #+#    #+#             */
/*   Updated: 2026/06/04 20:42:03 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer_internal.h"

static void	define_rules(t_rules *rules)
{
	if (!rules)
		return ;
	rules->start_word = (L_WORD | P_DQUOTE | P_SQUOTE | P_SYMBOL);
	rules->start_operator = (L_PIPE | L_REDIR_IN | L_REDIR_OUT);
}

void	rules_init(t_rules *rules)
{
	if (!rules)
		return ;
	scanner_table_char(&rules->table);
	scanner_table_token(&rules->table);
	define_rules(rules);
}
