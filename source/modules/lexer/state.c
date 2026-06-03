#include "../../headers/lexer_internal.h"

int	state_machine(t_manager *manager, t_scanner *input, t_rules *rules)
{
	unsigned int	props;

	if (!input)
		return (1);
	while (!scanner_isend(input))
	{
		props = rules->table.props[scanner_current(input)];
		if (props & rules->start_operator)
			extract_getoperator(manager, input, rules);
		else if (props & rules->start_word)
		{
			if (!extract_getword(manager, input, rules))
				return (0);
		}
		else
			scanner_advance(input);
	}
	return (1);
}
