/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 22:09:36 by g-alves-          #+#    #+#             */
/*   Updated: 2026/05/23 18:10:06 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/private/lexer_internal.h"

static void			ft_state_machine(t_manager *manager,
						t_scanner *input, t_rules *rules);
static t_list_token	*get_word(t_manager *manager, t_scanner *input,
						t_rules *rules);
static t_list_token	*get_operator(t_manager *manager, t_scanner *input,
						t_rules *rules);

t_manager	*lexer_controll(t_scanner	*input)
{
	t_manager		*manager;
	t_rules			*rules;

	if (!init_lexer_variable(&manager, &rules))
		return (NULL);
	define_rules(rules);
	ft_state_machine(manager, input, rules);
	return (manager);
}

static void	ft_state_machine(t_manager *manager, t_scanner *input,
	t_rules *rules)
{
	unsigned int	props;

	if (!input)
		return ;
	while (!scanner_is_end(input))
	{
		props = 0;
		props = rules->table.props[scanner_current(input)];
		if (props & rules->start_operator)
			get_operator(manager, input, rules);
		else if (props & rules->start_word)
			get_word(manager, input, rules);
		else
			scanner_advance(input);
	}
}

static t_list_token	*get_word(t_manager *manager, t_scanner *input,
		t_rules *rules)
{
	unsigned int	state;
	unsigned int	props;
	char			*word;
	t_list_token	*token;

	state = P_NONE;
	scanner_mark_start(input);
	while (!scanner_is_end(input))
	{
		props = rules->table.props[scanner_current(input)];
		update_quote_state(props, &state);
		if ((state == P_NONE) && (props & (P_SPACE | rules->start_operator)))
			break ;
		scanner_advance(input);
	}
	// if (state != P_NONE)
	// {
	// 	write (1, "Error", 5);
	// 	return (0);
	// }
	word = scanner_extract(input);
	token = add_token_to_list(manager, word, TOKEN_WORD);
	free(word);
	return (token);
}

static t_list_token	*get_operator(t_manager *manager, t_scanner *input,
						t_rules *rules)
{
	unsigned int	props;
	char			*type;
	t_list_token	*token;
	unsigned int	count;

	count = 0;
	scanner_mark_start(input);
	while (!scanner_is_end(input) && count < 2)
	{
		props = rules->table.props[scanner_current(input)];
		if ((!(props & rules->start_operator)))
			break ;
		scanner_advance(input);
		count++;
	}
	type = scanner_extract(input);
	token = add_token_to_list(manager, type, define_type(type));
	free(type);
	return (token);
}
