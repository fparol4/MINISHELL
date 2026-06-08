/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:16 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:16 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../_bin.h"

static void	export_sortkeys(char **keys, int size)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strcmp(keys[j], keys[j + 1]) > 0)
			{
				tmp = keys[j];
				keys[j] = keys[j + 1];
				keys[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

static int	export_key_count(t_env **env)
{
	int		size;
	t_env	*node;

	size = 0;
	node = *env;
	while (node)
	{
		if (ft_strcmp(node->key, ENV_ERRCODE) != 0)
			size++;
		node = node->next;
	}
	return (size);
}

static void	export_fill_keys(t_env **env, char **keys)
{
	int		i;
	t_env	*node;

	i = 0;
	node = *env;
	while (node)
	{
		if (ft_strcmp(node->key, ENV_ERRCODE) != 0)
			keys[i++] = node->key;
		node = node->next;
	}
}

char	**export_getkeys(t_env **env, int *size_out)
{
	char	**keys;

	*size_out = export_key_count(env);
	keys = ft_calloc(*size_out + 1, sizeof(char *));
	if (!keys)
		return (NULL);
	export_fill_keys(env, keys);
	export_sortkeys(keys, *size_out);
	return (keys);
}
