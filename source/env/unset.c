/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:46:45 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 18:46:45 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./_env.h"

int	env_unset(t_env **env, const char *key)
{
	t_env	*prev;
	t_env	*node;

	if (!env || !*env || !key)
		return (0);
	prev = NULL;
	node = *env;
	while (node)
	{
		if (ft_strcmp(node->key, key) == 0)
		{
			if (prev)
				prev->next = node->next;
			else
				*env = node->next;
			env_node_free(node);
			return (0);
		}
		prev = node;
		node = node->next;
	}
	return (0);
}
