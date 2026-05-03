# include "../types/envm.h"
# include "../../lib/libft/libft.h"
# include <stdlib.h>

void	envm_unset(t_env **env_list, const char *key)
{
	t_env	*prev;
	t_env	*node;
	size_t	key_len;

	if (!env_list || !*env_list || !key)
		return ;
	key_len = ft_strlen(key);
	prev = NULL;
	node = *env_list;
	while (node)
	{
		if (ft_strncmp(node->key, key, key_len + 1) == 0)
		{
			if (prev)
				prev->next = node->next;
			else
				*env_list = node->next;
			free(node->key);
			free(node->value);
			free(node);
			return ;
		}
		prev = node;
		node = node->next;
	}
}
