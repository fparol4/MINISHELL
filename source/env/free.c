/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:34:14 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/10 16:34:14 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./_env.h"

int	env_free(t_env **env)
{
	t_env	*node;
	t_env	*next;

	if (!env || !*env)
		return (0);
	node = *env;
	while (node)
	{
		next = node->next;
		env_node_free(node);
		node = next;
	}
	*env = NULL;
	return (0);
}
