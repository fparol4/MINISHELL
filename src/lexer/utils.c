/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 01:19:06 by g-alves-          #+#    #+#             */
/*   Updated: 2026/03/27 22:47:00 by g-alves-         ###   ########.fr       */
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

