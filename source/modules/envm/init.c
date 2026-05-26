/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 14:18:59 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/25 14:18:59 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./common.c"
#include "../../../headers/minishell.h"

static void	f_shlvl(t_env *env)
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

static t_env	*f_entryparse(const char *entry)
{
	char	*eq;
	char	*key;
	char	*value;
	t_env	*node;
	size_t	key_l;

	eq = ft_strchr(entry, '=');
	if (!eq)
		return (f_newnode(entry, NULL));
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
	node = f_newnode(key, value);
	free(key);
	free(value);
	return (node);
}

static char	**f_minenv(void)
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

t_env	*env_init(char **env)
{
	int		i;
	t_env	*head;
	t_env	*tail;
	t_env	*node;

	if (!env)
		return (env_init(f_minenv()));
	i = 0;
	head = NULL;
	tail = NULL;
	while (env[i])
	{
		node = f_entryparse(env[i]);
		if (!node)
		{
			env_free(&head);
			return (NULL);
		}
		if (!head)
			head = node;
		else
			tail->next = node;
		tail = node;
		i++;
	}
	f_shlvl(head);
	return (head);
}
