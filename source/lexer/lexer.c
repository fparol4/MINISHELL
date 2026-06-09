/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:09:16 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:09:16 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_lexer.h"

static int	lexer_init(t_manager **manager, t_rules **rules)
{
	*manager = ft_dlist_new(0, NULL);
	if (!*manager)
		return (0);
	*rules = malloc(sizeof(t_rules));
	if (!*rules)
	{
		free(*manager);
		*manager = NULL;
		return (0);
	}
	return (1);
}

void	lexer_free(t_manager *manager)
{
	t_node	*current;
	t_node	*next;

	if (!manager)
		return ;
	current = manager->head;
	while (current)
	{
		next = current->next;
		token_free(current->content);
		free(current);
		current = next;
	}
	free(manager);
}

static t_manager	*lexer_cleanup(t_manager *manager, t_rules *rules)
{
	free(rules);
	lexer_free(manager);
	return (NULL);
}

t_manager	*lexer(const char *input)
{
	t_scanner	sc;
	t_manager	*manager;
	t_rules		*rules;

	if (!input)
		return (NULL);
	scanner_init(&sc, input);
	if (!lexer_init(&manager, &rules))
		return (NULL);
	rules_init(rules);
	if (!state_machine(manager, &sc, rules))
	{
		sh_stxerr(SNTX_UNCLOSED_QUOTE);
		return (lexer_cleanup(manager, rules));
	}
	free(rules);
	return (manager);
}
