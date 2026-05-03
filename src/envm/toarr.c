# include "../types/envm.h"
# include "../../lib/libft/libft.h"
# include <stdlib.h>

static int	count_nodes(t_env *env_list)
{
	int		count;
	t_env	*node;

	count = 0;
	node = env_list;
	while (node)
	{
		count++;
		node = node->next;
	}
	return (count);
}

static char	*build_entry(const char *key, const char *value)
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

char	**envm_toarr(t_env *env_list)
{
	char	**arr;
	t_env	*node;
	int		count;
	int		i;

	count = count_nodes(env_list);
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	node = env_list;
	i = 0;
	while (node)
	{
		arr[i] = build_entry(node->key, node->value);
		if (!arr[i])
		{
			while (--i >= 0)
				free(arr[i]);
			free(arr);
			return (NULL);
		}
		i++;
		node = node->next;
	}
	arr[i] = NULL;
	return (arr);
}
