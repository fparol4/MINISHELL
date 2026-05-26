/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:42:39 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/26 15:42:39 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./common.h"

int	env_set(t_env **env, const char *key, const char *value)
{
	char	*vdup;
	size_t	key_l;
	t_env	*node;
	t_env	*tail;

	if (!env || !key)
		return (-1);
	key_l = ft_strlen(key);
	node = *env;
	while (node)
	{
		if (ft_strcmp(node->key, key) == 0)
		{
			if (value)
				vdup = ft_strdup(value);
			else
				vdup = NULL;
			if (value && !vdup)
				return (-1);
			free(node->value);
			node->value = vdup;
			return (0);
		}
		tail = node;
		node = node->next;
	}
	node = f_newnode(key, value);
	if (!node)
		return (-1);
	if (!*env)
		*env = node;
	else
		tail->next = node;
	return (0);
}
