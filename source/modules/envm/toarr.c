/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toarr.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:42:39 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/26 15:42:39 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

static char	*f_toentry(const char *key, const char *value)
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

static int	f_envsize(t_env **env)
{
	int		size;
	t_env	*node;

	size = 0;
	node = *env;
	while (node)
	{
		size++;
		node = node->next;
	}
	return (size);
}

char	**env_toarr(t_env **env)
{
	int		i;
	int		es;
	char	**arr;
	t_env	*node;

	es = f_envsize(env);
	arr = calloc(es + 1, sizeof(char *));
	if (!arr)
		return (NULL);
	node = *env;
	i = 0;
	while (node)
	{
		arr[i] = f_toentry(node->key, node->value);
		if (!arr[i])
		{
			sh_freeargs(arr);
			return (NULL);
		}
		node = node->next;
		i++;
	}
	return (arr);
}
