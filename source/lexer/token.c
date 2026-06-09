/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:09:16 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:09:16 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_lexer.h"

void	token_free(void *content)
{
	t_list_token	*token;

	token = (t_list_token *)content;
	if (token)
	{
		free(token->value);
		free(token);
	}
}

t_list_token	*token_add(t_manager *manager, const char *value,
		t_token_type type)
{
	t_node			*node;
	t_list_token	*token;

	token = malloc(sizeof(t_list_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
		return (free(token), NULL);
	token->type = type;
	token->expand = (ft_strchr(value, '$') != NULL);
	token->quoted = sh_has_quotes((char *)value);
	node = ft_dlist_node_new(token);
	if (!node)
		return (free(token->value), free(token), NULL);
	ft_dlist_add_tail(manager, node);
	return (token);
}
