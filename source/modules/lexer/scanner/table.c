#include "../header.h"

void	scanner_table_char(t_scanner_table *table)
{
	int	i;

	if (!table)
		return ;
	i = 0;
	while (i < 256)
		table->props[i++] = 0;
	i = 0;
	while (i < 256)
	{
		if (ft_isalpha(i))
			table->props[i] |= P_ALPHA;
		else if (ft_isdigit(i))
			table->props[i] |= P_DIGIT;
		else if ((i >= 9 && i <= 13) || i == 32)
			table->props[i] |= P_SPACE;
		else if (i == '\'')
			table->props[i] |= P_SQUOTE;
		else if (i == '"')
			table->props[i] |= P_DQUOTE;
		else if (i == '\\')
			table->props[i] |= P_ESCAPE;
		else
			table->props[i] |= P_SYMBOL;
		i++;
	}
}

void	scanner_table_token(t_scanner_table *table)
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
