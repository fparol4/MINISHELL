/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:17 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:17 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "./lexer.h"

typedef struct s_command	t_command;
typedef struct s_parser		t_parser;

typedef enum e_syntax_error
{
	SNTX_OK,
	SNTX_UNCLOSED_QUOTE,
	SNTX_PIPE_START,
	SNTX_PIPE_END,
	SNTX_EMPTY_CMD,
	SNTX_REDIR_NO_TARGET
}	t_syntax_error;

typedef struct s_ast
{
	t_command		*root;
	size_t			size;
	int				error;
	t_syntax_error	error_type;
}	t_ast;

typedef enum e_parser_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_parser_redir_type;

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
	t_array	args;
	t_array	redirs;
}	t_simple;

typedef struct s_pipe
{
	struct s_command	*right;
	struct s_command	*left;
}	t_pipe;

typedef struct s_command
{
	t_pnode_type	type;
	union u_define
	{
		t_simple	simple;
		t_pipe		pipe;
	}	t_define;
}	t_command;

t_ast	*parser_controller(t_manager *manager);
void	parser_free_ast(t_ast *ast);
void	parser_free_cmd(t_command *cmd);

#endif
