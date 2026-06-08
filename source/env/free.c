/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 12:55:14 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 12:55:14 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./_env.h"

int	env_free(t_env **env)
{
	t_env	*node;
	t_env	*next;

	if (!env || !*env)
		return (1);
	node = *env;
	while (node)
	{
		next = node->next;
		env_node_free(node);
		node = next;
	}
	*env = NULL;
	return (1);
}
