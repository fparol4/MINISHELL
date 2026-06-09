/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:21:56 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:21:56 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_lexer.h"

static void	quote_update(unsigned int props, unsigned int *state)
{
	if ((props & P_SQUOTE) && *state == P_NONE)
		*state = P_SQUOTE;
	else if ((props & P_DQUOTE) && *state == P_NONE)
		*state = P_DQUOTE;
	else if ((props & P_SQUOTE) && *state == P_SQUOTE)
		*state = P_NONE;
	else if ((props & P_DQUOTE) && *state == P_DQUOTE)
		*state = P_NONE;
}

static int	word_isend(unsigned int props, unsigned int state, t_rules *rules)
{
	if (state != P_NONE)
		return (0);
	return ((props & P_SPACE) || (props & rules->start_operator));
}

static t_token_type	operator_type(const char *type)
{
	if (ft_strcmp(type, ">>") == 0)
		return (TOKEN_APPEND);
	if (ft_strcmp(type, "<<") == 0)
		return (TOKEN_HEREDOC);
	if (ft_strcmp(type, "|") == 0)
		return (TOKEN_PIPE);
	if (ft_strcmp(type, "<") == 0)
		return (TOKEN_REDIR_IN);
	if (ft_strcmp(type, ">") == 0)
		return (TOKEN_REDIR_OUT);
	return (TOKEN_NONE);
}

t_list_token	*extract_word(t_manager *manager, t_scanner *input,
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
		quote_update(props, &state);
		if (word_isend(props, state, rules))
			break ;
		scanner_advance(input);
	}
	word = scanner_extract(input);
	if (!input || input->cursor <= input->start)
		return (free(word), NULL);
	if (state != P_NONE)
		return (free(word), NULL);
	token = token_add(manager, word, TOKEN_WORD);
	free(word);
	return (token);
}

t_list_token	*extract_operator(t_manager *manager, t_scanner *input,
		t_rules *rules)
{
	unsigned int	props;
	char			*type;
	t_list_token	*token;
	unsigned int	count;

	count = 0;
	scanner_mark_start(input);
	while (!scanner_is_end(input) && count < OPERATOR_MAX_LEN)
	{
		props = rules->table.props[scanner_current(input)];
		if (!(props & rules->start_operator))
			break ;
		scanner_advance(input);
		count++;
	}
	type = scanner_extract(input);
	if (!input || input->cursor <= input->start)
		return (free(type), NULL);
	token = token_add(manager, type, operator_type(type));
	free(type);
	return (token);
}
