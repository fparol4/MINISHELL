/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _parser.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 00:00:00 by g-alves-          #+#    #+#             */
/*   Updated: 2026/06/08 00:00:00 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODULES_PARSER_H
# define MODULES_PARSER_H

# include "../../../headers/parser.h"

# define OFFSET 2

typedef enum e_token_class
{
	C_NONE = 0,
	C_WORD = 1 << (OFFSET + 0),
	C_REDIR = 1 << (OFFSET + 1),
	C_PIPE = 1 << (OFFSET + 2)
}	t_token_class;

typedef struct s_parser
{
	t_node			*current;
	t_list_token	*token;
	t_token_type	last_type;
	unsigned int	state;
	t_simple		*current_cmd;
	t_ast			*ast;
}	t_parser;

t_list_token	*parser_get_token(t_node *current_node);
unsigned int	parser_get_class(t_token_type type);

t_command		*parse_simple_command(t_parser *parser);
void			parser_free_simple(t_simple *simple);
void			parser_set_syntax_error(t_parser *parser, t_syntax_error type);
void			parser_init(t_parser *parser, t_manager *manager);
void			parser_next(t_parser *parser);
t_bool			parser_is_end(t_parser *parser);
void			parser_start_command(t_parser *parser);
void			parser_finish_command(t_parser *parser);
void			parser_add_arg(t_parser *parser, char *arg, t_bool expand);
void			parser_add_redir(t_parser *parser, t_parser_redir_type type,
					t_list_token *token);

#endif
