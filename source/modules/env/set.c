/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 14:06:40 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/31 14:06:40 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./env.h"

int	env_set(t_env **env, const char *key, const char *value)
{
	char	*vdup;
	t_env	*node;
	t_env	*tail;

	if (!env || !key)
		return (-1);
	node = *env;
	while (node)
	{
		if (ft_strcmp(node->key, key) == 0)
		{
			if (!value)
				return (0);
			vdup = ft_strdup(value);
			if (!vdup)
				return (-1);
			free(node->value);
			node->value = vdup;
			return (0);
		}
		tail = node;
		node = node->next;
	}
	node = env_node_new(key, value);
	if (!node)
		return (-1);
	if (!*env)
		*env = node;
	else
		tail->next = node;
	return (0);
}
