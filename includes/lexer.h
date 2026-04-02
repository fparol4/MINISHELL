/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 18:49:16 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/01 19:27:03 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "minishell.h"

typedef enum e_token_type
{
	TOKEN_PIPE = 1,
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

t_manager		*lexer_controll(char	**input);
t_list_token	*add_token_to_list(t_manager *manager, char *value,
					t_token_type type);
void			ft_print_list(void *content);
char			is_quote(char **input, char quote);
char			*ft_remove_char(char *str, char c);

#endif