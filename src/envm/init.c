# include "../types/envm.h"
# include "../../lib/libft/libft.h"
# include <stdlib.h>
# include <unistd.h>

static t_env	*new_node(const char *key, const char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
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
	else
		node->value = NULL;
	if (value && !node->value)
	{
		free(node->key);
		free(node);
		return (NULL);
	}
	node->next = NULL;
	return (node);
}

static t_env	*parse_entry(const char *entry)
{
	char	*eq;
	char	*key;
	char	*value;
	t_env	*node;
	size_t	key_len;

	eq = ft_strchr(entry, '=');
	if (!eq)
		return (new_node(entry, NULL));
	key_len = (size_t)(eq - entry);
	key = ft_substr(entry, 0, key_len);
	if (!key)
		return (NULL);
	value = ft_strdup(eq + 1);
	if (!value)
	{
		free(key);
		return (NULL);
	}
	node = new_node(key, value);
	free(key);
	free(value);
	return (node);
}

static void	increment_shlvl(t_env *env_list)
{
	t_env	*node;
	int		lvl;
	char	*new_val;

	node = env_list;
	while (node)
	{
		if (ft_strncmp(node->key, "SHLVL", 6) == 0)
		{
			lvl = ft_atoi(node->value ? node->value : "0") + 1;
			new_val = ft_itoa(lvl);
			free(node->value);
			node->value = new_val;
			return ;
		}
		node = node->next;
	}
}

static t_env	*minimal_env(void)
{
	t_env	*head;
	t_env	*shlvl;
	char	buf[1024];
	char	*cwd;

	cwd = getcwd(buf, sizeof(buf));
	head = new_node("PWD", cwd ? cwd : "");
	if (!head)
		return (NULL);
	shlvl = new_node("SHLVL", "1");
	if (!shlvl)
	{
		free(head->key);
		free(head->value);
		free(head);
		return (NULL);
	}
	head->next = shlvl;
	return (head);
}

t_env	*envm_init(char **envp)
{
	t_env	*head;
	t_env	*tail;
	t_env	*node;
	int		i;

	if (!envp || !envp[0])
		return (minimal_env());
	head = NULL;
	tail = NULL;
	i = 0;
	while (envp[i])
	{
		node = parse_entry(envp[i]);
		if (!node)
		{
			envm_free(&head);
			return (NULL);
		}
		if (!head)
			head = node;
		else
			tail->next = node;
		tail = node;
		i++;
	}
	increment_shlvl(head);
	return (head);
}
