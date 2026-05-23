/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 01:19:06 by g-alves-          #+#    #+#             */
/*   Updated: 2026/05/23 18:11:54 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/private/lexer_internal.h"

void	update_quote_state(unsigned int props, unsigned int *state)
{
	if ((props & P_SQUOTE) && *state == P_NONE)
		*state = P_SQUOTE;
	else if ((props & P_DQUOTE) && (*state == P_NONE))
		*state = P_DQUOTE;
	else if (((props & P_SQUOTE) && (*state == P_SQUOTE))
		|| (((props & P_DQUOTE) && (*state == P_DQUOTE))))
		*state = P_NONE;
}

void	init_token_table(t_char_table *table)
{
	int	i;

	i = 0;
	while (i < 256)
	{
		if (ft_isalpha(i) || ft_isdigit(i))
			table->props[i] |= L_WORD;
		else if (i == '|')
			table->props[i] |= L_PIPE;
		else if (i == '>')
			table->props[i] |= L_REDIR_IN;
		else if (i == '<')
			table->props[i] |= L_REDIR_OUT;
		else if (i == '$')
			table->props[i] |= L_DOLAR;
		else
			table->props[i] |= L_NONE;
		i++;
	}
}

int	init_lexer_variable(t_manager **manager, t_rules **rules)
{
	*manager = malloc(sizeof(t_manager));
	if (!*manager)
		return (0);
	(*manager)->head = NULL;
	(*manager)->tail = NULL;
	*rules = malloc(sizeof(t_rules));
	if (!*rules)
	{
		free(*manager);
		return (0);
	}
	init_char_table(&(*rules)->table);
	init_token_table(&(*rules)->table);
	return (1);
}

void	define_rules(t_rules *rules)
{
	rules->start_word = (L_WORD | P_DQUOTE | P_SQUOTE | P_SYMBOL);
	rules->start_operator = (L_PIPE | L_REDIR_IN | L_REDIR_OUT);
}

t_token_type	define_type(char *type)
{
	if (ft_strncmp(type, ">>", 2) == 0)
		return (TOKEN_APPEND);
	if (ft_strncmp(type, "<<", 2) == 0)
		return (TOKEN_HEREDOC);
	if (ft_strncmp(type, "|", 1) == 0)
		return (TOKEN_PIPE);
	if (ft_strncmp(type, "<", 1) == 0)
		return (TOKEN_REDIR_IN);
	if (ft_strncmp(type, ">", 1) == 0)
		return (TOKEN_REDIR_OUT);
	return (0);
}
