#include "../../headers/lexer_internal.h"

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
		*manager = NULL;
		return (0);
	}
	init_char_table(&(*rules)->table);
	init_token_table(&(*rules)->table);
	return (1);
}
