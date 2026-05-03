# include "../types/envm.h"
# include "../../lib/libft/libft.h"
# include <stdlib.h>

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

int	envm_set(t_env **env_list, const char *key, const char *value)
{
	t_env	*node;
	t_env	*tail;
	char	*dup;
	size_t	key_len;

	if (!env_list || !key)
		return (-1);
	key_len = ft_strlen(key);
	node = *env_list;
	while (node)
	{
		if (ft_strncmp(node->key, key, key_len + 1) == 0)
		{
			if (value)
				dup = ft_strdup(value);
			else
				dup = NULL;
			if (value && !dup)
				return (-1);
			free(node->value);
			node->value = dup;
			return (0);
		}
		tail = node;
		node = node->next;
	}
	node = new_node(key, value);
	if (!node)
		return (-1);
	if (!*env_list)
		*env_list = node;
	else
		tail->next = node;
	return (0);
}
