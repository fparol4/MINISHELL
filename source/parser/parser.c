/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:21:56 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:21:56 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_parser.h"

static t_command	*build_pipe_node(t_command *left, t_command *right)
{
	t_command	*node;

	node = ft_calloc(1, sizeof(t_command));
	if (!node)
		return (NULL);
	node->type = PNODE_PIPE;
	node->t_define.pipe.left = left;
	node->t_define.pipe.right = right;
	return (node);
}

static t_command	*build_pipeline(t_parser *parser)
{
	t_command	*left;
	t_command	*right;

	left = parse_simple_command(parser);
	if (!left || parser->state)
		return (NULL);
	while (parser->token && parser->token->type == TOKEN_PIPE)
	{
		parser_next(parser);
		if (parser_is_end(parser))
		{
			parser_set_syntax_error(parser, SNTX_PIPE_END);
			parser_free_cmd(left);
			return (NULL);
		}
		right = parse_simple_command(parser);
		if (!right || parser->state)
		{
			parser_free_cmd(left);
			return (NULL);
		}
		left = build_pipe_node(left, right);
	}
	return (left);
}

t_ast	*parser_controller(t_manager *manager)
{
	t_ast		*ast;
	t_parser	parser;

	ast = ft_calloc(1, sizeof(t_ast));
	if (!ast)
		return (NULL);
	parser_init(&parser, manager);
	if (parser_is_end(&parser))
		return (free(ast), NULL);
	parser.ast = ast;
	if (parser.token->type == TOKEN_PIPE)
	{
		parser_set_syntax_error(&parser, SNTX_PIPE_START);
		ast->error = 1;
		return (ast);
	}
	ast->root = build_pipeline(&parser);
	if (!ast->root)
		ast->error = 1;
	else
		ast->size = 1;
	return (ast);
}
