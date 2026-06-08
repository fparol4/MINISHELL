/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_state.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 20:43:54 by g-alves-          #+#    #+#             */
/*   Updated: 2026/06/04 20:43:54 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/parser_internal.h"

static size_t	str_array_len(char **arr)
{
	size_t	count;

	count = 0;
	if (arr)
		while (arr[count])
			count++;
	return (count);
}

void	parser_start_command(t_parser *parser)
{
	if (!parser)
		return ;
	if (!parser->current_cmd)
		parser->current_cmd = ft_calloc(1, sizeof(t_simple));
	if (!parser->current_cmd)
	{
		parser->state = TRUE;
		return ;
	}
	parser->current_cmd->args = NULL;
	parser->current_cmd->redirs = NULL;
}

void	parser_finish_command(t_parser *parser)
{
	if (!parser || !parser->current_cmd)
		return ;
	if (parser->state)
	{
		parser_free_simple(parser->current_cmd);
		parser->current_cmd = NULL;
		return ;
	}
	if (!parser->current_cmd->args && !parser->current_cmd->redirs)
	{
		parser_set_syntax_error(parser, SNTX_EMPTY_CMD);
		parser_free_simple(parser->current_cmd);
		parser->current_cmd = NULL;
		return ;
	}
	parser->current_cmd = NULL;
}

void	parser_add_arg(t_parser *parser, char *arg, t_bool expand)
{
	size_t	count;
	char	**new;

	if (!parser)
		return ;
	if (!arg)
	{
		parser->state = TRUE;
		return ;
	}
	if (!parser->current_cmd)
		parser_start_command(parser);
	if (!parser->current_cmd)
		return ;
	if (expand)
		parser->current_cmd->expand = TRUE;
	count = str_array_len(parser->current_cmd->args);
	new = dynarray_append(parser->current_cmd->args, sizeof(char *), &count);
	if (!new)
	{
		free(arg);
		parser->state = TRUE;
		return ;
	}
	new[count - 1] = arg;
	new[count] = NULL;
	parser->current_cmd->args = new;
}

void	parser_add_redir(t_parser *parser, t_parser_redir_type type,
	t_list_token *token)
{
	t_parser_redir	*new;
	char			*word;
	size_t			count;

	if (!parser || !token)
		return ;
	if (!parser->current_cmd)
		parser_start_command(parser);
	if (!parser->current_cmd)
		return ;
	word = ft_strdup(token->value);
	if (!word)
	{
		parser->state = TRUE;
		return ;
	}
	count = parser->current_cmd->redir_count;
	new = dynarray_append(parser->current_cmd->redirs,
			sizeof(t_parser_redir), &count);
	if (!new)
	{
		free(word);
		parser->state = TRUE;
		return ;
	}
	parser->current_cmd->redir_count = count;
	new[count - 1].type = type;
	new[count - 1].file = word;
	new[count - 1].quoted = token->quoted;
	new[count - 1].expand = !token->quoted;
	parser->current_cmd->redirs = new;
}
