/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 01:19:06 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/30 10:34:03 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list_token	*add_token_to_list(t_manager *manager, char *value,
	t_token_type type)
{
	t_node			*node;
	t_list_token	*token;

	token = malloc(sizeof(t_list_token));
	token->value = ft_strdup((const char *)value);
	token->type = type;

	node = ft_create_node(token);
	ft_dlist_add_back(manager, node);
	return (token);
}

void	ft_print_list(void *content)
{
	t_list_token	*token;

	token = (t_list_token *)content;
	ft_printf("the type is: %i | ", token->type);
	ft_printf("the content is: %s\n", token->value);
}

void	update_qupte_state(unsigned int props, unsigned int *state)
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

char	*ft_remove_char(char *str, char c)
{
	char	*new_str;
	size_t	len;
	size_t	index;

	if (!str || !c)
		return (NULL);
	len = 0;
	index = 0;
	while (str[index])
	{
		if (*str != c)
			len++;
		index++;
	}
	new_str = malloc((len + 1) * sizeof(char)); // lembrar de faze rum safe_calloc pra facilitar a vida
	index = 0;
	len = 0;
	while (str[len])
	{
		if (str[len] != c)
			new_str[index++] = str[len];
		len++;
	}
	new_str[index] = '\0';
	return (new_str);
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
