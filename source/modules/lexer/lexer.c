#include "../../headers/lexer_internal.h"

static int	ft_state_machine(t_manager *manager, t_scanner *input,
				t_rules *rules)
{
	unsigned int	props;

	if (!input)
		return (1);
	while (!scanner_is_end(input))
	{
		props = rules->table.props[scanner_current(input)];
		if (props & rules->start_operator)
			get_operator(manager, input, rules);
		else if (props & rules->start_word)
		{
			if (!get_word(manager, input, rules))
				return (0);
		}
		else
			scanner_advance(input);
	}
	return (1);
}

t_manager	*lexer_control(t_scanner *input)
{
	t_manager	*manager;
	t_rules		*rules;

	if (!init_lexer_variable(&manager, &rules))
		return (NULL);
	define_rules(rules);
	if (!ft_state_machine(manager, input, rules))
	{
		print_syntax_error(SNTX_UNCLOSED_QUOTE);
		free(rules);
		lexer_free(manager);
		return (NULL);
	}
	free(rules);
	return (manager);
}
