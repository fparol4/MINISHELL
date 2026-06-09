/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:21:58 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:21:58 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../_bin.h"

static void	export_write_node(t_env *node)
{
	ft_putstr_fd("declare -x ", 1);
	ft_putstr_fd(node->key, 1);
	if (node->value)
	{
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd(node->value, 1);
		ft_putchar_fd('"', 1);
	}
	ft_putchar_fd('\n', 1);
}

static t_env	*export_find(t_env **env, char *key)
{
	t_env	*node;

	node = *env;
	while (node && ft_strcmp(node->key, key))
		node = node->next;
	return (node);
}

int	export_print(t_env **env)
{
	int		i;
	int		k_size;
	t_env	*node;
	char	**keys;

	keys = export_getkeys(env, &k_size);
	if (!keys)
		return (1);
	i = 0;
	while (keys[i])
	{
		node = export_find(env, keys[i]);
		if (node)
			export_write_node(node);
		i++;
	}
	free(keys);
	return (0);
}
