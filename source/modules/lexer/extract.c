#include "../../headers/lexer_internal.h"

t_list_token	*get_word(t_manager *manager, t_scanner *input, t_rules *rules)
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
	word = scanner_extract(input);
	if (state != P_NONE)
		return (free(word), NULL);
	token = add_token_to_list(manager, word, TOKEN_WORD);
	free(word);
	return (token);
}

t_list_token	*get_operator(t_manager *manager, t_scanner *input,
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
		if (!(props & rules->start_operator))
			break ;
		scanner_advance(input);
		count++;
	}
	type = scanner_extract(input);
	token = add_token_to_list(manager, type, define_type(type));
	free(type);
	return (token);
}
