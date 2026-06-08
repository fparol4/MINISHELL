/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:14 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:14 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./_env.h"

static int	env_update_value(t_env *node, const char *value)
{
	char	*vdup;

	if (!value)
		return (0);
	vdup = ft_strdup(value);
	if (!vdup)
		return (-1);
	free(node->value);
	node->value = vdup;
	return (0);
}

static int	env_append_node(t_env **env, t_env *tail, const char *key,
		const char *value)
{
	t_env	*node;

	node = env_node_new(key, value);
	if (!node)
		return (-1);
	if (!*env)
		*env = node;
	else
		tail->next = node;
	return (0);
}

int	env_set(t_env **env, const char *key, const char *value)
{
	t_env	*node;
	t_env	*tail;

	if (!env || !key)
		return (-1);
	node = *env;
	tail = NULL;
	while (node)
	{
		if (ft_strcmp(node->key, key) == 0)
			return (env_update_value(node, value));
		tail = node;
		node = node->next;
	}
	return (env_append_node(env, tail, key, value));
}
