/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 20:56:05 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/30 10:40:32 by g-alves-         ###   ########.fr       */
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
# include "../libft/linked_list/header.h"
# include "lexer.h"
# include "../infra/infra.h"

typedef struct s_rules
{
	t_char_table	table;
	unsigned int	is_word;
	unsigned int	is_operator;
}	t_rules;

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

#endif