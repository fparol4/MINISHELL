/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toarr.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 14:06:40 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/31 14:06:40 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./modules_env.h"

static char	*toentry(const char *key, const char *value)
{
	char	*tmp;
	char	*entry;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	if (value)
		entry = ft_strjoin(tmp, value);
	else
		entry = ft_strdup(tmp);
	free(tmp);
	return (entry);
}

static int	env_fill_entry(t_env *node, char **arr, int i)
{
	arr[i] = toentry(node->key, node->value);
	if (!arr[i])
	{
		sh_freeargs(arr);
		return (1);
	}
	return (0);
}

char	**env_toarr(t_env **env)
{
	int		i;
	int		es;
	char	**arr;
	t_env	*node;

	es = env_size(env);
	arr = calloc(es + 1, sizeof(char *));
	if (!arr)
		return (NULL);
	node = *env;
	i = 0;
	while (node)
	{
		if (node->value && ft_strcmp(node->key, ENV_ERRCODE) != 0)
		{
			if (env_fill_entry(node, arr, i))
				return (NULL);
			i++;
		}
		node = node->next;
	}
	return (arr);
}
