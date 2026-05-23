/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 13:37:03 by g-alves-          #+#    #+#             */
/*   Updated: 2026/05/23 18:21:27 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_INTERNAL_H
# define PARSER_INTERNAL_H

# include "includes/public/parser.h"
# include "../infra/infra.h"

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*word;
	t_bool			expand;
}	t_redir;

typedef struct s_simple
{
	char	**args;
	t_redir	*redirs;
}	t_simple;

typedef struct s_pipe
{
	struct s_command	*right;
	struct s_command	*left;
}	t_pipe;

typedef struct s_command
{
	t_node_type	type;
	union	u_define
	{
		t_simple	simple;
		t_pipe		pipe;
	}	t_define;
}	t_command;

typedef struct s_ast
{
	t_command	*root;
	size_t		size;
	int			error;
}	t_ast;

#endif