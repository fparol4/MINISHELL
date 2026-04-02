/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 01:19:06 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/01 15:12:39 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list_token	*add_token_to_list(t_manager *manager, char *value,
	t_token_type type)
{
	t_node			*node;
	t_list_token	*token;

	token = malloc(sizeof(t_list_token));
	token->value = value;
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

char	is_quote(char **input, char quote)
{
	if (**input == '\'' || **input == '"')
		quote = **input;
	(*input)++;
	if (**input == quote)
	{
		quote = '\0';
		(*input)++;
	}
	return (quote);
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
