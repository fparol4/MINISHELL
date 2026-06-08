/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 18:53:47 by g-alves-          #+#    #+#             */
/*   Updated: 2026/06/04 18:54:10 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "../libraries/libft/libft.h"

typedef struct s_dlist_node	t_node;
typedef struct s_dlist		t_manager;

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

typedef struct s_list_token
{
	char			*value;
	t_token_type	type;
	t_bool			expand;
	t_bool			quoted;
}	t_list_token;

t_manager	*lexer(const char *input);
void		lexer_free(t_manager *manager);

#endif
