/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 14:06:39 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/31 14:06:39 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

static int	f_vid(char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	f_sortkeys(char **keys, int size)
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

static char	**f_getkeys(t_env **env, int size)
{
	int		i;
	t_env	*node;
	char	**keys;

	keys = ft_calloc(size + 1, sizeof(char *));
	if (!keys)
		return (NULL);
	i = 0;
	node = *env;
	while (node)
	{
		keys[i++] = node->key;
		node = node->next;
	}
	f_sortkeys(keys, size);
	return (keys);
}

static int	f_print(t_env **env)
{
	int		i;
	int		k_size;
	t_env	*node;
	char	**keys;

	k_size = env_size(env);
	keys = f_getkeys(env, k_size);
	if (!keys)
		return (1);
	i = 0;
	while (keys[i])
	{
		node = *env;
		while (node && ft_strcmp(node->key, keys[i]))
			node = node->next;
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(keys[i], 1);
		if (node && node->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(node->value, 1);
			ft_putstr_fd("\"", 1);
		}
		ft_putchar_fd('\n', 1);
		i++;
	}
	free(keys);
	return (0);
}

int	bin_export(char **args, t_env **env)
{
	int		i;
	int		code;
	char	*key;
	char	*c_eq;

	code = 0;
	if (!args[0])
		return (f_print(env));
	i = 0;
	while (args[i])
	{
		if (!f_vid(args[i]))
		{
			ft_putstr_fd("minishell: export: '", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			code = 1;
		}
		else
		{
			c_eq = ft_strchr(args[i], '=');
			if (c_eq)
			{
				key = ft_substr(args[i], 0, c_eq - args[i]);
				env_set(env, key, c_eq + 1);
				free(key);
			}
			else
				env_set(env, args[i], NULL);
		}
		i++;
	}
	return (code);
}
