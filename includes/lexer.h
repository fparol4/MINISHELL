/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 18:49:16 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/27 14:49:08 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

# define OFFSET 8

typedef enum e_token_type
{
	TOKEN_NONE,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_WORD
}	t_token_type;

typedef enum e_lexer_table
{
	L_WORD = 1 << (OFFSET + 0),
	L_REDIR_IN = 1 << (OFFSET + 1),
	L_REDIR_OUT = 1 << (OFFSET + 2),
	L_DOLAR = 1 << (OFFSET + 3),
	L_SQUOTE = 1 << (OFFSET + 4),
	L_DQUOTE = 1 << (OFFSET + 5),
	L_PIPE = 1 << (OFFSET + 6)
}	t_lexer_table;

typedef struct s_token_table
{
	unsigned int	token_props[256];
}	t_token_table;

typedef enum e_state
{
	S_NORMAL = 1 << 1,
	S_IN_SQUOTE = 1 << 2,
	S_IN_DQUOTE = 1 << 3,
}	t_state;

typedef struct s_list_token
{
	char				*value;
	t_token_type		type;
}	t_list_token;

t_manager		*lexer_controll(char	*i_scanner);
t_list_token	*add_token_to_list(t_manager *manager, char *value,
					t_token_type type);
void			ft_print_list(void *content);
char			is_quote(char *i_scanner, char quote);
char			*ft_remove_char(char *str, char c);
void			init_token_table(t_token_table *table);
int				get_operator(int operator);

#endif