/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:15 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:15 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_parser.h"

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
	if (ft_array_init(&parser->current_cmd->args, sizeof(char *))
		|| ft_array_init(&parser->current_cmd->redirs, sizeof(t_parser_redir)))
		parser->state = TRUE;
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
	if (!parser->current_cmd->args.length
		&& !parser->current_cmd->redirs.length)
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
	char	**args;

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
	if (!ft_array_append(&parser->current_cmd->args, &arg))
	{
		free(arg);
		parser->state = TRUE;
		return ;
	}
	args = (char **)parser->current_cmd->args.items;
	args[parser->current_cmd->args.length] = NULL;
}

void	parser_add_redir(t_parser *parser, t_parser_redir_type type,
		t_list_token *token)
{
	t_parser_redir	redir;
	char			*word;

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
	redir.type = type;
	redir.file = word;
	redir.quoted = token->quoted;
	redir.expand = !token->quoted;
	if (!ft_array_append(&parser->current_cmd->redirs, &redir))
	{
		free(word);
		parser->state = TRUE;
		return ;
	}
}
