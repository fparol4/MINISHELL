/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 18:52:07 by g-alves-          #+#    #+#             */
/*   Updated: 2026/06/04 18:52:08 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_INTERNAL_H
# define PARSER_INTERNAL_H

# include "./parser.h"

# define OFFSET 2

typedef enum e_token_class
{
	C_NONE = 0,
	C_WORD = 1 << (OFFSET + 0),
	C_REDIR = 1 << (OFFSET + 1),
	C_PIPE = 1 << (OFFSET + 2)
}	t_token_class;

typedef enum e_pnode_type
{
	PNODE_CMD,
	PNODE_PIPE
}	t_pnode_type;

typedef struct s_parser_redir
{
	t_parser_redir_type	type;
	char				*file;
	t_bool				expand;
	t_bool				quoted;
}	t_parser_redir;

typedef struct s_simple
{
	char				**args;
	t_bool				expand;
	t_parser_redir		*redirs;
	size_t				redir_count;
}	t_simple;

typedef struct s_pipe
{
	struct s_command	*right;
	struct s_command	*left;
}	t_pipe;

typedef struct s_command
{
	t_pnode_type	type;
	union	u_define
	{
		t_simple	simple;
		t_pipe		pipe;
	}	t_define;
}	t_command;

typedef struct s_parser
{
	t_node			*current;
	t_list_token	*token;
	t_token_type	last_type;
	unsigned int	state;
	t_simple		*current_cmd;
	t_ast			*ast;
}	t_parser;

void			*dynarray_append(void *arr, size_t elem_size, size_t *count);
t_list_token	*get_token(t_node *current_node);
t_command		*parse_simple_command(t_parser *parser);
void			parser_free_command(t_command *cmd);
void			parser_free_simple(t_simple *simple);
void			parser_set_syntax_error(t_parser *parser, t_syntax_error type);
unsigned int	token_class(t_token_type type);
void			parser_init(t_parser *parser, t_manager *manager);
void			parser_next(t_parser *parser);
t_bool			parser_is_end(t_parser *parser);
void			parser_start_command(t_parser *parser);
void			parser_finish_command(t_parser *parser);
void			parser_add_arg(t_parser *parser, char *arg, t_bool expand);
void			parser_add_redir(t_parser *parser, t_parser_redir_type type,
					t_list_token *token);

#endif
