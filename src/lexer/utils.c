/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 01:19:06 by g-alves-          #+#    #+#             */
/*   Updated: 2026/05/11 14:55:52 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_list(void *content)
{
	t_list_token	*token;

	token = (t_list_token *)content;
	ft_printf("the type is: %i | ", token->type);
	ft_printf("the content is: %s\n", token->value);
}

void	update_quote_state(unsigned int props, unsigned int *state)
{
	if ((props & P_SQUOTE) && *state == P_NONE)
		*state = P_SQUOTE;
	else if ((props & P_DQUOTE) && (*state == P_NONE))
		*state = P_DQUOTE;
	else if (((props & P_SQUOTE) && (*state == P_SQUOTE))
		|| (((props & P_DQUOTE) && (*state == P_DQUOTE))))
		*state = P_NONE;
}

void	init_token_table(t_char_table *table)
{
	int	i;

	i = 0;
	while (i < 256)
	{
		if (ft_isalpha(i) || ft_isdigit(i))
			table->props[i] |= L_WORD;
		else if (i == '|')
			table->props[i] |= L_PIPE;
		else if (i == '>')
			table->props[i] |= L_REDIR_IN;
		else if (i == '<')
			table->props[i] |= L_REDIR_OUT;
		else if (i == '$')
			table->props[i] |= L_DOLAR;
		else
			table->props[i] |= L_NONE;
		i++;
	}
}

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
		return (0);
	}
	init_char_table(&(*rules)->table);
	init_token_table(&(*rules)->table);
	return (1);
}

void	define_rules(t_rules *rules)
{
	rules->start_word = (L_WORD | P_DQUOTE | P_SQUOTE | P_SYMBOL);
	rules->start_operator = (L_PIPE | L_REDIR_IN | L_REDIR_OUT);
}

// char	*ft_remove_char(char *str, char c)
// {
// 	char	*new_str;
// 	size_t	len;
// 	size_t	index;

// 	if (!str || !c)
// 		return (NULL);
// 	len = 0;
// 	index = 0;
// 	while (str[index])
// 	{
// 		if (*str != c)
// 			len++;
// 		index++;
// 	}
// 	new_str = malloc((len + 1) * sizeof(char));
// 	index = 0;
// 	len = 0;
// 	while (str[len])
// 	{
// 		if (str[len] != c)
// 			new_str[index++] = str[len];
// 		len++;
// 	}
// 	new_str[index] = '\0';
// 	return (new_str);
// }
