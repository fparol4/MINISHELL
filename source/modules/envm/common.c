/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:42:39 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/26 15:42:39 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./common.h"

t_env	*f_newnode(const char *key, const char *value)
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

int	f_freenode(t_env *node)
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
