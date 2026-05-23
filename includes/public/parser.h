/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 15:22:38 by g-alves-          #+#    #+#             */
/*   Updated: 2026/05/23 19:07:41 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

typedef struct s_command	t_command;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
}	t_node_type;

typedef struct s_parser
{
	t_node	*current;
}	t_parser;

t_command	*create_node_cmd(char **argv);
t_command	*creat_node_pipe(t_command *left, t_command *right);
t_command	*get_pipe_left(t_command *new_node);
t_command	*get_pipe_right(t_command *new_node);
t_node_type	get_type(t_command *new_node);

#endif