#include "../../headers/lexer_internal.h"

static void	update_quote_state(unsigned int props, unsigned int *state)
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

static int	word_should_stop(unsigned int props, unsigned int state,
				t_rules *rules)
{
	if (state != P_NONE)
		return (0);
	return ((props & P_SPACE) || (props & rules->start_operator));
}

t_list_token	*extract_getword(t_manager *manager, t_scanner *input,
					t_rules *rules)
{
	unsigned int	state;
	unsigned int	props;
	char			*word;
	t_list_token	*token;

	state = P_NONE;
	scanner_markstart(input);
	while (!scanner_isend(input))
	{
		props = rules->table.props[scanner_current(input)];
		update_quote_state(props, &state);
		if (word_should_stop(props, state, rules))
			break ;
		scanner_advance(input);
	}
	word = scanner_extract(input);
	if (state != P_NONE)
		return (free(word), NULL);
	token = token_add(manager, word, TOKEN_WORD);
	free(word);
	return (token);
}

t_list_token	*extract_getoperator(t_manager *manager, t_scanner *input,
					t_rules *rules)
{
	unsigned int	props;
	char			*type;
	t_list_token	*token;
	unsigned int	count;

	count = 0;
	scanner_markstart(input);
	while (!scanner_isend(input) && count < OPERATOR_MAX_LEN)
	{
		props = rules->table.props[scanner_current(input)];
		if (!(props & rules->start_operator))
			break ;
		scanner_advance(input);
		count++;
	}
	type = scanner_extract(input);
	token = token_add(manager, type, define_type(type));
	free(type);
	return (token);
}
