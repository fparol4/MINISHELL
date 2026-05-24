/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 15:22:01 by g-alves-          #+#    #+#             */
/*   Updated: 2026/05/24 18:33:16 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*create_node_cmd(char **argv)
{
	
}

t_command	*creat_node_pipe(t_command *left, t_command *right);
t_command	*get_pipe_left(t_command *new_node);
t_command	*get_pipe_right(t_command *new_node);
t_node_type	get_type(t_command *new_node);
