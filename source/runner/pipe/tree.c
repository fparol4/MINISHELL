/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:23:40 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:23:40 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_pipe.h"

size_t	rn_pipe_count(t_command *node)
{
	if (!node)
		return (0);
	if (node->type == PNODE_PIPE)
		return (rn_pipe_count(node->t_define.pipe.left)
			+ rn_pipe_count(node->t_define.pipe.right));
	return (1);
}

void	rn_pipe_flatten(t_command *node, t_command **items, size_t *idx)
{
	if (!node)
		return ;
	if (node->type == PNODE_PIPE)
	{
		rn_pipe_flatten(node->t_define.pipe.left, items, idx);
		rn_pipe_flatten(node->t_define.pipe.right, items, idx);
		return ;
	}
	items[(*idx)++] = node;
}
