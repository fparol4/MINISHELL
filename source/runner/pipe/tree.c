/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:46:46 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 18:46:46 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_pipe.h"

size_t	rn_pipe_count(t_command *node)
{
	size_t	count;

	if (!node)
		return (0);
	if (node->type == PNODE_PIPE)
	{
		count = rn_pipe_count(node->t_define.pipe.left);
		count += rn_pipe_count(node->t_define.pipe.right);
		return (count);
	}
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
