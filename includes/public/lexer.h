/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 18:49:16 by g-alves-          #+#    #+#             */
/*   Updated: 2026/05/23 18:19:46 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

typedef struct s_manager	t_manager;
typedef struct s_scanner	t_scanner;

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
}	t_list_token;

t_manager	*lexer_controll(t_scanner *scanner);

#endif