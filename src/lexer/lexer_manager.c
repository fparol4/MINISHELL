/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 14:18:13 by g-alves-          #+#    #+#             */
/*   Updated: 2026/05/04 14:18:16 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list_token	*add_token_to_list(t_manager *manager, char *value,
	t_token_type type)
{
	t_node			*node;
	t_list_token	*token;

	token = malloc(sizeof(t_list_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup((const char *)value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	node = ft_create_node(token);
	if (!node)
	{
		free(token->value);
		free(token);
		return (NULL);
	}
	ft_dlist_add_back(manager, node);
	return (token);
}
