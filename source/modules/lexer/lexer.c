#include "../../headers/lexer_internal.h"

static int	init_lexer_variable(t_manager **manager, t_rules **rules)
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

t_manager	*lexer(t_scanner *input)
{
	t_manager	*manager;
	t_rules		*rules;

	if (!init_lexer_variable(&manager, &rules))
		return (NULL);
	rules_init(rules);
	if (!state_machine(manager, input, rules))
	{
		sh_stxerr(SNTX_UNCLOSED_QUOTE);
		free(rules);
		lexer_free(manager);
		return (NULL);
	}
	free(rules);
	return (manager);
}
