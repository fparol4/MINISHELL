#include "../../headers/lexer_internal.h"

void	init_token_table(t_char_table *table)
{
	int	i;

	if (!table)
		return ;
	i = 0;
	while (i < 256)
	{
		if (ft_isalpha(i) || ft_isdigit(i))
			table->props[i] |= L_WORD;
		else if (i == '|')
			table->props[i] |= L_PIPE;
		else if (i == '>')
			table->props[i] |= L_REDIR_OUT;
		else if (i == '<')
			table->props[i] |= L_REDIR_IN;
		else if (i == '$')
			table->props[i] |= L_DOLLAR;
		else
			table->props[i] |= L_NONE;
		i++;
	}
}
