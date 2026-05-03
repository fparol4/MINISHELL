# include "../types/envm.h"
# include "../../lib/libft/libft.h"

char	*envm_get(t_env *env_list, const char *key)
{
	t_env	*node;
	size_t	key_len;

	if (!env_list || !key)
		return (NULL);
	key_len = ft_strlen(key);
	node = env_list;
	while (node)
	{
		if (ft_strncmp(node->key, key, key_len + 1) == 0)
			return (node->value);
		node = node->next;
	}
	return (NULL);
}
