#include "../../headers/lexer_internal.h"

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
	return (TOKEN_NONE);
}

static void	define_rules(t_rules *rules)
{
	if (!rules)
		return ;
	rules->start_word = (L_WORD | P_DQUOTE | P_SQUOTE | P_SYMBOL);
	rules->start_operator = (L_PIPE | L_REDIR_IN | L_REDIR_OUT);
}

void	rules_init(t_rules *rules)
{
	if (!rules)
		return ;
	scanner_tablechar(&rules->table);
	scanner_tabletoken(&rules->table);
	define_rules(rules);
}
