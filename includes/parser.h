/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 15:22:38 by g-alves-          #+#    #+#             */
/*   Updated: 2026/05/11 18:50:43 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PARSER_H
# define PARSER_H

typedef struct s_list_token	t_list_token;
typedef struct s_ast		t_ast;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_REDIR,
}	t_node_type;

typedef struct s_ast
{
	t_list_token		*token;
	t_node_type			type;
	t_ast				*left;
	t_ast				*right;
}	t_ast;

#endif