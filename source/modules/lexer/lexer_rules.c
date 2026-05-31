#include "../../headers/lexer_internal.h"

void	define_rules(t_rules *rules)
{
	if (!rules)
		return ;
	rules->start_word = (L_WORD | P_DQUOTE | P_SQUOTE | P_SYMBOL);
	rules->start_operator = (L_PIPE | L_REDIR_IN | L_REDIR_OUT);
}
