/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:15 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:15 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_parser.h"

static t_command	*simple_to_command(t_simple *simple)
{
	t_command	*cmd;

	if (!simple)
		return (NULL);
	cmd = ft_calloc(1, sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->type = PNODE_CMD;
	cmd->t_define.simple = *simple;
	free(simple);
	return (cmd);
}

static void	consume_word(t_parser *parser)
{
	parser_add_arg(parser, ft_strdup(parser->token->value),
		parser->token->expand);
	parser_next(parser);
}

static void	consume_redir(t_parser *parser)
{
	t_parser_redir_type	type;

	type = (t_parser_redir_type)(parser->token->type - TOKEN_REDIR_IN);
	parser_next(parser);
	if (!parser->token || parser_get_class(parser->token->type) != C_WORD)
	{
		parser_set_syntax_error(parser, SNTX_REDIR_NO_TARGET);
		return ;
	}
	parser_add_redir(parser, type, parser->token);
	parser_next(parser);
}

t_command	*parse_simple_command(t_parser *parser)
{
	t_simple		*simple;
	unsigned int	class;

	parser_start_command(parser);
	if (parser->state)
		return (NULL);
	while (parser->token && parser->token->type != TOKEN_PIPE)
	{
		class = parser_get_class(parser->token->type);
		if (class == C_WORD)
			consume_word(parser);
		else if (class == C_REDIR)
			consume_redir(parser);
		else
		{
			parser_set_syntax_error(parser, SNTX_EMPTY_CMD);
			return (NULL);
		}
	}
	simple = parser->current_cmd;
	parser_finish_command(parser);
	if (parser->state || !simple)
		return (NULL);
	return (simple_to_command(simple));
}
