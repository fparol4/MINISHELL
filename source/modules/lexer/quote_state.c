#include "../../headers/lexer_internal.h"

void	update_quote_state(unsigned int props, unsigned int *state)
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
