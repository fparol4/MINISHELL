/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 15:18:45 by g-alves-          #+#    #+#             */
/*   Updated: 2026/05/24 18:33:26 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list_token	*get_token(t_node *current_node);
static t_command	*create_cmd_node(t_manager *manager, t_node *current_node,
						t_list_token *token);


int	parser_controller(t_manager *manager)
{
	t_node			*current_node;
	t_list_token	*token;

	current_node = manager->head;
	while (current_node)
	{
		token = get_token(current_node);
		if (token->type != TOKEN_PIPE)
			create_cmd_node(manager, current_node, token);
		else
			create_pipe_node();
		current_node = current_node->next;
	}
}

static t_list_token	*get_token(t_node *current_node)
{
	if (!current_node)
		return (NULL);
	return ((t_list_token *)current_node->content);
}

static t_command	*create_cmd_node(t_manager *manager, t_node *current_node,
						t_list_token *token)
{
	int	count;

	count = 0;
	while (current_node && token->type != TOKEN_PIPE)
	{
		while (token->type == TOKEN_WORD)
		{
			guard_info();
			count++;
			current_node = current_node->next;
			token = get_token(current_node);
		}
		while (token->type & (TOKEN_REDIR_IN | TOKEN_REDIR_OUT
				| TOKEN_APPEND | TOKEN_HEREDOC))
		{
			create_redir_list();
			count++;
			current_node = current_node->next;
			token = get_token(current_node);
		}
	}
	if (token->type == TOKEN_PIPE)
		return (NULL);
}
