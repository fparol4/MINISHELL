/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/24 20:56:05 by g-alves-          #+#    #+#             */
/*   Updated: 2026/02/24 21:49:49 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef enum e_type
{
	PIPE,
	I_REDIRECT,
	O_REDIRECT,
	APPEND,
	HEREDOC,
	WORD,
	ARGUMENT,
	COMAND,
}	t_type;

typedef struct s_token
{
	int		id;
	char	**token;
	t_type	type;
}	t_token;

typedef struct s_AST
{

}	t_AST;

#endif