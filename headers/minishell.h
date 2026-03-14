/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 20:56:05 by g-alves-          #+#    #+#             */
/*   Updated: 2026/03/09 21:05:21 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

typedef enum e_token_type
{
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_WORD,
}	t_token_type;

typedef struct s_list_token
{
	char				*value;
	t_token_type		type;
}	t_list_token;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_REDIR,
}	t_node_type;

typedef struct s_ast	t_ast;
struct s_ast
{
	t_list_token		*token;
	t_node_type			type;
	t_ast				*left;
	t_ast				*right;
};

t_list_token	*lexer_controll(char	*input);
void			*add_token_to_list(t_manager *manager, char *value,
					t_token_type type);

#endif