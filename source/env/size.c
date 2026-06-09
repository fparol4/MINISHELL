/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   size.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:46:45 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 18:46:45 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./_env.h"

int	env_size(t_env **envm)
{
	int		size;
	t_env	*node;

	if (!envm)
		return (0);
	size = 0;
	node = *envm;
	while (node)
	{
		size++;
		node = node->next;
	}
	return (size);
}
