/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 20:15:16 by g-alves-          #+#    #+#             */
/*   Updated: 2026/06/04 20:15:16 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./modules_env.h"

t_env	*env_node_new(const char *key, const char *value)
{
	t_env	*node;

	node = calloc(1, sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (!node->key)
	{
		free(node);
		return (NULL);
	}
	if (value)
		node->value = ft_strdup(value);
	if (value && !node->value)
	{
		free(node->key);
		free(node);
		return (NULL);
	}
	return (node);
}

int	env_node_free(t_env *node)
{
	if (!node)
		return (0);
	if (node->key)
		free(node->key);
	if (node->value)
		free(node->value);
	free(node);
	return (0);
}
