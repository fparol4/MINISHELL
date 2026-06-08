/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:14 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:14 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./_env.h"

static void	shlvl(t_env *env)
{
	int		shlvl;
	t_env	*node;

	node = env;
	while (node)
	{
		if (ft_strcmp(node->key, "SHLVL") == 0)
		{
			if (!node->value)
				shlvl = 1;
			else
				shlvl = ft_atoi(node->value) + 1;
			free(node->value);
			node->value = ft_itoa(shlvl);
			return ;
		}
		node = node->next;
	}
}

static t_env	*entryparse(const char *entry)
{
	char	*eq;
	char	*key;
	char	*value;
	t_env	*node;
	size_t	key_l;

	eq = ft_strchr(entry, '=');
	if (!eq)
		return (env_node_new(entry, NULL));
	key_l = (size_t)(eq - entry);
	key = ft_substr(entry, 0, key_l);
	if (!key)
		return (NULL);
	value = ft_strdup(eq + 1);
	if (!value)
	{
		free(key);
		return (NULL);
	}
	node = env_node_new(key, value);
	free(key);
	free(value);
	return (node);
}

static char	**minenv(void)
{
	char	cwd[1024];
	char	**env;

	if (!getcwd(cwd, sizeof(cwd)))
		return (NULL);
	env = ft_calloc(3, sizeof(char *));
	if (!env)
		return (NULL);
	env[0] = ft_strjoin("PWD=", cwd);
	env[1] = ft_strdup("SHLVL=1");
	if (!env[0] || !env[1])
	{
		free(env[0]);
		free(env[1]);
		free(env);
		return (NULL);
	}
	return (env);
}

static t_env	*env_list_from_array(char **env)
{
	int		i;
	t_env	*head;
	t_env	**next;

	i = 0;
	head = NULL;
	next = &head;
	while (env[i])
	{
		*next = entryparse(env[i]);
		if (!*next)
		{
			env_free(&head);
			return (NULL);
		}
		next = &(*next)->next;
		i++;
	}
	*next = NULL;
	return (head);
}

t_env	*env_init(char **env)
{
	t_env	*head;

	if (!env)
		return (env_init(minenv()));
	head = env_list_from_array(env);
	if (!head)
		return (NULL);
	shlvl(head);
	return (head);
}
